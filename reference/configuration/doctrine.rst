.. index::
   single: Doctrine; ORM Configuration Reference
   single: Configuration Reference; Doctrine ORM

Konfiguracja
============

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection:   default
                connections:
                    default:
                        dbname:               database
                        host:                 localhost
                        port:                 1234
                        user:                 user
                        password:             secret
                        driver:               pdo_mysql
                        driver_class:         MyNamespace\MyDriverImpl
                        options:
                            foo: bar
                        path:                 %kernel.data_dir%/data.sqlite
                        memory:               true
                        unix_socket:          /tmp/mysql.sock
                        wrapper_class:        MyDoctrineDbalConnectionWrapper
                        charset:              UTF8
                        logging:              %kernel.debug%
                        platform_service:     MyOwnDatabasePlatformService
                        mapping_types:
                            enum: string
                    conn1:
                        # ...
                types:
                    custom: Acme\HelloBundle\MyCustomType
            orm:
                auto_generate_proxy_classes:    false
                proxy_namespace:                Proxies
                proxy_dir:                      %kernel.cache_dir%/doctrine/orm/Proxies
                default_entity_manager:         default # The first defined is used if not set
                entity_managers:
                    default:
                        # Nazwa połączenia DBAL (połączenie oznaczone atrybutem "default" jest połączeniem domyślnym)
                        connection:                     conn1
                        mappings: # Required
                            AcmeHelloBundle: ~
                        class_metadata_factory_name:    Doctrine\ORM\Mapping\ClassMetadataFactory
                        # All cache drivers have to be array, apc, xcache or memcache
                        metadata_cache_driver:          array
                        query_cache_driver:             array
                        result_cache_driver:
                            type:           memcache
                            host:           localhost
                            port:           11211
                            instance_class: Memcache
                            class:          Doctrine\Common\Cache\MemcacheCache
                        dql:
                            string_functions:
                                test_string: Acme\HelloBundle\DQL\StringFunction
                            numeric_functions:
                                test_numeric: Acme\HelloBundle\DQL\NumericFunction
                            datetime_functions:
                                test_datetime: Acme\HelloBundle\DQL\DatetimeFunction
                    em2:
                        # ...

    .. code-block:: xml

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
                            dir="%kernel.root_dir%/../src/vendor/DoctrineExtensions/lib/DoctrineExtensions/Entity"
                            prefix="DoctrineExtensions\Entity"
                            alias="DExt"
                        />
                    </doctrine:entity-manager>
                </doctrine:orm>
            </doctrine:config>
        </container>

Przegląd Konfiguracji
---------------------

Poniższy przykład konfiguracji pokazuje wszystkie domyślne ustawienia konfiguracji ORMa:

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            # standardowa dystrybucja nadpisuje tą wartość, ustawiając true w trybie debug, oraz false w przeciwnym wypadku
            auto_generate_proxy_classes: false
            proxy_namespace: Proxies
            proxy_dir: %kernel.cache_dir%/doctrine/orm/Proxies
            default_entity_manager: default
            metadata_cache_driver: array
            query_cache_driver: array
            result_cache_driver: array

Istnieje jeszcze wiele innych opcji konfiguracyjnych których możesz użyć do zastąpienia niektórych klas,
ale jest to przewidziane do bardziej zaawansowanych przypadków użycia.

Sterowniki Cache
~~~~~~~~~~~~~~~~

Dla sterowników cache możesz ustwić następujące wartości "array", "apc", "memcache"
lub "xcache".

Poniższy przykład pokazuje nadpisanie konfiguracji cache:

.. code-block:: yaml

    doctrine:
        orm:
            auto_mapping: true
            metadata_cache_driver: apc
            query_cache_driver: xcache
            result_cache_driver:
                type: memcache
                host: localhost
                port: 11211
                instance_class: Memcache

Konfiguracja Mapowania
~~~~~~~~~~~~~~~~~~~~~~

Wyraźna definicja wszystkich zmapowanych encji jest potrzebna tylko w konfiguracji
ORM, jest tam kilka opcji które możesz kontrolować. Poniższe opcje konfiguracyjne
dostępne są dla mapowania:

* ``type`` przyjmuje wartości ``annotation``, ``xml``, ``yml``, ``php`` lub ``staticphp``.
  Opcja ta określa jaki sposób wykorzystujesz podczas opisywania mapowań.

* ``dir`` ścieżka do mapowań lub też plików encji (w zależności od sterownika). 
  Jeśli ścieżka jest względna, to zakładamy że jest względna do głównego katalogu bundla.
  Działa to tylko wtedy gdy nazwa Twojego mapowania jest identyczna z nazwą bundla. 
  Jeśli chcesz użyć ścieżki absolutnej powinieneś poprzedzić ją parametrem z kernela,
  który jest dostępny w DIC (np. %kernel.root_dir%).

* ``prefix`` Wspólny prefiks które wszystkie encje z tego mapowania będą używać.
  Prefiks ten nie powinien być definiowany dla kilku różnych mapowań. Ponieważ
  niektóre z Twoich encji mogą nie zostać znalezione przez Doctrine.
  Ta opcja domyślnie przyjmuje wartość przestrzeń nazw bundla + ``Enity``, dla przykładu
  dla aplikacji bundla nazwanej ``AcmeHelloBundle`` prefix będzie wyglądał tak
  ``Acme\HelloBundle\Entity``.

* ``alias`` Doctrine umożliwia skrócenie przestrzeni nazw encji, do krótszej wersji
  używanej przez zapytania DQL lub też "Repository", poprzez skróty. Gdy używasz bundla
  domyślnym aliasem jest nazwa bundla.

* ``is_bundle`` Opcja ta wywodzi się z opcji ``dir`` i domyślnie przyjmuje wartość true
  jeśli folder jest względny, poprzez sprawdzenie czy ``file_exists()`` zwraca false.
  Przyjmuje wartość false jeśli sprawdzenie istnienia zwraca true. W tym przypadku
  ścieżka absolutna została określona i pliki metadanych w większości przypadków są poza
  folderem bundla.

.. index::
    single: Configuration; Doctrine DBAL
    single: Doctrine; DBAL configuration

.. _`reference-dbal-configuration`:


Konfiguracja Doctrine DBAL
--------------------------

.. note::

    DoctrineBundle wspiera wszystkie parametry które są akceptowane przez sterowniki
    Doctrine, przekonwertowane na XML lub YAML w formacie egzekwowanym przez Symfony.
    Zobacz dokumentację Doctrine `DBAL documentation`_ w celu uzyskania większej ilości
    informacji.

Poza tym domyślne opcje Doctrine są powiązane z opcjami Symfony które możesz konfigurować.
Poniższy przykład pokazuje wszystkie możliwe opcje konfiguracyjne:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                dbname:               database
                host:                 localhost
                port:                 1234
                user:                 user
                password:             secret
                driver:               pdo_mysql
                driver_class:         MyNamespace\MyDriverImpl
                options:
                    foo: bar
                path:                 %kernel.data_dir%/data.sqlite
                memory:               true
                unix_socket:          /tmp/mysql.sock
                wrapper_class:        MyDoctrineDbalConnectionWrapper
                charset:              UTF8
                logging:              %kernel.debug%
                platform_service:     MyOwnDatabasePlatformService
                mapping_types:
                    enum: string
                types:
                    custom: Acme\HelloBundle\MyCustomType

    .. code-block:: xml

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

Jeśli chcesz skonfigurować kilka połączeń w YAML, zdefiniuj je w kolekcji ``connections`
oraz nadaj im unikalną nazwę:

.. code-block:: yaml

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

Usługa ``database_connection`` zawsze odwołuje się do *domyślnego* połączenia,
który to jest pierwszym zdefiniowanym lub też oznaczonym parametrem ``default_connection``.

Każde z połączeń jest także dostępne poprzez usługę ``doctrine.dbal.[name]_connection``
gdzie ``[name]`` jest nazwą połączenia.

.. _DBAL documentation: http://www.doctrine-project.org/docs/dbal/2.0/en
