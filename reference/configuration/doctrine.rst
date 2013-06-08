.. index::
   pair: Doctrine; konfiguracja


Konfiguracja Doctrine
=====================

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        doctrine:
            dbal:
                default_connection:   default
                types:
                    # Kolekcja własnych typów
                    # Na przykład
                    some_custom_type:
                        class:                Acme\HelloBundle\MyCustomType
                        commented:            true

                connections:
                    default:
                        dbname:               database

                    # Kolekcja różnych nazwanych połączeń (np. default, conn2 itd.)
                    default:
                        dbname:               ~
                        host:                 localhost
                        port:                 ~
                        user:                 root
                        password:             ~
                        charset:              ~
                        path:                 ~
                        memory:               ~

                        # gnazdo Unix używane przez MySQL
                        unix_socket:          ~

                        # True jeśli ma być stosowane stałe połączenie dla sterownika ibm_db2
                        persistent:           ~

                        # Protokół jaki ma być stosowany dla sterownika ibm_db2 (jeśli nie podano, to domyślnie użyty będzie TCP/IP)
                        protocol:             ~

                        # True jeśli ma być użyta jako nazwa usługi nazwa bazy danyc zamiast SID dla Oracle
                        service:              ~

                        # Tryb sesji do zastosowania dla sterownika oci8
                        sessionMode:          ~

                        # True jeśli ze sterownikiem oci8 ma być użyty zbiorczy serwer
                        pooled:               ~

                        # Konfiguracja MultipleActiveResultSets dla sterownika pdo_sqlsrv
                        MultipleActiveResultSets:  ~
                        driver:               pdo_mysql
                        platform_service:     ~
                        logging:              %kernel.debug%
                        profiling:            %kernel.debug%
                        driver_class:         ~
                        wrapper_class:        ~
                        options:
                            # tablica ocji
                            key:                  []
                        mapping_types:
                            # tablica typów mapowania
                            name:                 []
                        slaves:

                            # kolekcja nazwanych połączeń podrzędnych (np. slave1, slave2)
                            slave1:
                                dbname:               ~
                                host:                 localhost
                                port:                 ~
                                user:                 root
                                password:             ~
                                charset:              ~
                                path:                 ~
                                memory:               ~

                                # gniazdo unix do zastosowania przez MySQL
                                unix_socket:          ~

                                # True jeśli ma być użyte stałe połączenie dla sterownika ibm_db2
                                persistent:           ~

                                # Protokół jaki ma być użyty dla sterownika ibm_db2 (domyślnie jest to TCP/IP)
                                protocol:             ~

                                # True jeśli jako nazwa usługi ma być użyta nazwa bazy danych zamiast SID dla Oracle
                                service:              ~

                                # Tryb sesji do uzycia dla sterownika oci8
                                sessionMode:          ~

                                # True jeśli dla sterownika oci8 ma być użyty zbiorczy serwer
                                pooled:               ~

                                # Configuring MultipleActiveResultSets dla sterownika pdo_sqlsrv
                                MultipleActiveResultSets:  ~

            orm:
                default_entity_manager:  ~
                auto_generate_proxy_classes:  false
                proxy_dir:            %kernel.cache_dir%/doctrine/orm/Proxies
                proxy_namespace:      Proxies
                # poszukaj na ten temat informacji o klasie "ResolveTargetEntityListener" w Receptariuszu
                resolve_target_entities: []
                entity_managers:
                    # Kolekcja róznych nazwanych menadżerów encji (np. some_em, another_em)
                    some_em:
                        query_cache_driver:
                            type:                 array # wymagane
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        metadata_cache_driver:
                            type:                 array # wymagane
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        result_cache_driver:
                            type:                 array # wymagane
                            host:                 ~
                            port:                 ~
                            instance_class:       ~
                            class:                ~
                        connection:           ~
                        class_metadata_factory_name:  Doctrine\ORM\Mapping\ClassMetadataFactory
                        default_repository_class:  Doctrine\ORM\EntityRepository
                        auto_mapping:         false
                        hydrators:

                            # Tablica nazw hydratorów
                            hydrator_name:                 []
                        mappings:
                            # Tablica odwzorowań, którymi muszą być nazwy pakietów lub coś innego
                            mapping_name:
                                mapping:              true
                                type:                 ~
                                dir:                  ~
                                alias:                ~
                                prefix:               ~
                                is_bundle:            ~
                        dql:
                            # kolekcja funkcji łańcuchowych
                            string_functions:
                                # przykład
                                # test_string: Acme\HelloBundle\DQL\StringFunction

                            # kolekcja funkcji numerycznych
                            numeric_functions:
                                # przykład
                                # test_numeric: Acme\HelloBundle\DQL\NumericFunction

                            # kolekcja funkcji daty i czasu
                            datetime_functions:
                                # przykład
                                # test_datetime: Acme\HelloBundle\DQL\DatetimeFunction

                        # zaresjestrowanie filtrów SQL dla menadżera encji
                        filters:
                            # Tablica filtrów
                            some_filter:
                                class:                ~ # wymagane
                                enabled:              false

    .. code-block:: xml
       :linenos:
       
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection
                        name="default"
                        dbname="database"
                        host="localhost"
                        port="1234"
                        user="user"
                        password="secret"
                        driver="pdo_mysql"
                        driver-class="MyNamespace\MyDriverImpl"
                        path="%kernel.data_dir%/data.sqlite"
                        memory="true"
                        unix-socket="/tmp/mysql.sock"
                        wrapper-class="MyDoctrineDbalConnectionWrapper"
                        charset="UTF8"
                        logging="%kernel.debug%"
                        platform-service="MyOwnDatabasePlatformService"
                    >
                        <doctrine:option key="foo">bar</doctrine:option>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                    <doctrine:connection name="conn1" />
                    <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
                </doctrine:dbal>

                <doctrine:orm default-entity-manager="default" auto-generate-proxy-classes="false" proxy-namespace="Proxies" proxy-dir="%kernel.cache_dir%/doctrine/orm/Proxies">
                    <doctrine:entity-manager name="default" query-cache-driver="array" result-cache-driver="array" connection="conn1" class-metadata-factory-name="Doctrine\ORM\Mapping\ClassMetadataFactory">
                        <doctrine:metadata-cache-driver type="memcache" host="localhost" port="11211" instance-class="Memcache" class="Doctrine\Common\Cache\MemcacheCache" />
                        <doctrine:mapping name="AcmeHelloBundle" />
                        <doctrine:dql>
                            <doctrine:string-function name="test_string>Acme\HelloBundle\DQL\StringFunction</doctrine:string-function>
                            <doctrine:numeric-function name="test_numeric>Acme\HelloBundle\DQL\NumericFunction</doctrine:numeric-function>
                            <doctrine:datetime-function name="test_datetime>Acme\HelloBundle\DQL\DatetimeFunction</doctrine:datetime-function>
                        </doctrine:dql>
                    </doctrine:entity-manager>
                    <doctrine:entity-manager name="em2" connection="conn2" metadata-cache-driver="apc">
                        <doctrine:mapping
                            name="DoctrineExtensions"
                            type="xml"
                            dir="%kernel.root_dir%/../vendor/gedmo/doctrine-extensions/lib/DoctrineExtensions/Entity"
                            prefix="DoctrineExtensions\Entity"
                            alias="DExt"
                        />
                    </doctrine:entity-manager>
                </doctrine:orm>
            </doctrine:config>
        </container>


Przegląd Konfiguracji
---------------------

Poniższy przykład konfiguracji pokazuje wszystkie domyślne ustawienia konfiguracji
rozpoznawane przez ORM:

.. code-block:: yaml
   :linenos:

    doctrine:
        orm:
            auto_mapping: true
            # standardowa dystrybucja nadpisuje tą wartość, ustawiając true w trybie debugowania a false w przeciwnym wypadku
            auto_generate_proxy_classes: false
            proxy_namespace: Proxies
            proxy_dir: "%kernel.cache_dir%/doctrine/orm/Proxies"
            default_entity_manager: default
            metadata_cache_driver: array
            query_cache_driver: array
            result_cache_driver: array

Istnieje jeszcze wiele innych opcji konfiguracyjnych których możesz użyć do
zastąpienia niektórych klas, ale jest to już zastosowanie bardzo zaawansowane.

Sterowniki buforowania
~~~~~~~~~~~~~~~~~~~~~~

Dla sterowników buforowania można ustawić następujące wartości "array", "apc",
"memcache" lub "xcache".

Poniższy przykład pokazuje ogólny zarys konfiguracji buforowania:

.. code-block:: yaml
   :linenos:

    doctrine:
        orm:
            auto_mapping: true
            metadata_cache_driver: apc
            query_cache_driver:
                type: service
                id: my_doctrine_common_cache_service
            result_cache_driver:
                type: memcache
                host: localhost
                port: 11211
                instance_class: Memcache

Konfiguracja mapowania
~~~~~~~~~~~~~~~~~~~~~~

Niezbędną konfiguracją dla ORM jest tylko jawne zdefiniowanie wszystkich
odwzorowywanych dokumentów i ma ona kilka opcji konfiguracyjnych, które
można kontrolować. Dla odwzorowań istnieją następujące opcje konfiguracyjne:

* ``type``: przyjmuje wartości ``annotation``, ``xml``, ``yml``, ``php`` lub ``staticphp``.
  Opcja określa typ metadanych stosowany w mapowaniu.

* ``dir``: ścieżka do plików odwzorowań lub encji (w zależności od sterownika).
  Jeśli jest to ścieżka względna, to odnosi się ona do katalogu pakietu. Działa
  to tylko wtedy, gdy nazwa odwzorowań jest taka sama jak nazwa pakietu. Jeżeli
  chce się użyć tej opcji do określenia ścieżki bezwzględnej, to należy podać
  przedrostek ścieżki z parametrami *kernel*, które istnieją w DIC (na przykład
  %kernel.root_dir%).

* ``prefix``: wspólny przedrostek przestrzeni nazw dla wszystkich encji z tego
  udziału odwzorowań. Przedrostek ten nie powinien kolidować z przedrostkami innych
  definicji odwzorowań, gdyż w takim przypadku encje nie będą mogły być odnalezione
  przez Doctrine. Opcja ta domyślnie przyjmuje wartość nazwy pakietu + ``Entity``.
  Przykładowo, dla pakietu aplikacji o nazwie ``AcmeHelloBundle`` przedrostkiem będzie
  ``Acme\HelloBundle\Entity``.

* ``alias``: w celu uproszczenia, Doctrine oferuje możliwość aliasowanie nazw
  przestrzeni nazw encji przez używanie w zapytaniach DQL lub przy dostępie do
  repozytorium krótkich nazw. W przypadku używania pakietu, domyślna wartością
  aliasu jest nazwa pakietu.

* ``is_bundle`` wartość tej opcji jest pochodną wartością opcji ``dir`` i domyślnie
  jest to *true*, jeśli wartość ``dir`` jest adresem względnym dla którego funkcja
  ``file_exists()` zwraca *false*. Gdy sprawdzenie istnienia pliku zwraca *true*,
  to jest wartość *false*. W takim przypadku zostaje określona ścieżka bezwzględna
  a pliki metadanych prawdopodobnie znajdują się poza pakietem.

.. index::
    single: konfiguracja; Doctrine DBAL
    single: Doctrine; konfiguracja DBAL

.. _`reference-dbal-configuration`:


Konfiguracja Doctrine DBAL
--------------------------

DoctrineBundle obsługuje wszystkie parametry które są akceptowane przez sterowniki
Doctrine, przekonwertowane na standardy nazewnicze XML lub YAML egzekwowane przez
Symfony. Proszę przeczytać dokumentację Doctrine `DBAL documentation`_ w celu
uzyskania większej ilości informacji. Poniższy przykład pokazuje wszystkie możliwe
opcje konfiguracyjne:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        doctrine:
            dbal:
                dbname:               database
                host:                 localhost
                port:                 1234
                user:                 user
                password:             secret
                driver:               pdo_mysql
                # the DBAL driverClass option
                driver_class:         MyNamespace\MyDriverImpl
                # opcje DBAL driverOptions
                options:
                    foo: bar
                path:                 "%kernel.data_dir%/data.sqlite"
                memory:               true
                unix_socket:          /tmp/mysql.sock
                # opcje DBAL wrapperClass
                wrapper_class:        MyDoctrineDbalConnectionWrapper
                charset:              UTF8
                logging:              "%kernel.debug%"
                platform_service:     MyOwnDatabasePlatformService
                mapping_types:
                    enum: string
                types:
                    custom: Acme\HelloBundle\MyCustomType
                # opcje DBAL keepSlave
                keep_slave:           false

    .. code-block:: xml
       :linenos:

        <!-- xmlns:doctrine="http://symfony.com/schema/dic/doctrine" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd"> -->

        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="database"
                host="localhost"
                port="1234"
                user="user"
                password="secret"
                driver="pdo_mysql"
                driver-class="MyNamespace\MyDriverImpl"
                path="%kernel.data_dir%/data.sqlite"
                memory="true"
                unix-socket="/tmp/mysql.sock"
                wrapper-class="MyDoctrineDbalConnectionWrapper"
                charset="UTF8"
                logging="%kernel.debug%"
                platform-service="MyOwnDatabasePlatformService"
            >
                <doctrine:option key="foo">bar</doctrine:option>
                <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                <doctrine:type name="custom">Acme\HelloBundle\MyCustomType</doctrine:type>
            </doctrine:dbal>
        </doctrine:config>

Jeżeli w pliku YAML chce się skonfigurować wiele połączeń, należy je umieścić w
kluczu ``connections`` i nadać im unikalna nazwę:

.. code-block:: yaml
   :linenos:

    doctrine:
        dbal:
            default_connection:       default
            connections:
                default:
                    dbname:           Symfony2
                    user:             root
                    password:         null
                    host:             localhost
                customer:
                    dbname:           customer
                    user:             root
                    password:         null
                    host:             localhost

Usługa ``database_connection`` zawsze odnosi się do połączenia *default*,
które jest skonfigurowane pierwsze lub połączenia skonfigurowanego w parametrze
``default_connection``.

Każde z połączeń jest także dostępne poprzez usługę ``doctrine.dbal.[name]_connection``
gdzie ``[name]`` jest nazwą połączenia.

.. _DBAL documentation: http://www.doctrine-project.org/docs/dbal/2.0/en
