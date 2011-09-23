.. index::
   single: Session; Database Storage

Jak korzystać z PdoSessionStorage do przechowywania danych sesji w bazie danych
===============================================================================

Domyślne przechowywanie sesji w Symfony2 opiera się na plikach.
Większość średnich i dużych witryn internetowych wykorzystuje bazy 
danych do przechowywania sesji, zamiast plików. Bazy danych są łatwiejsze 
w użyciu i prościej daje się je skalować w środowiskach multi-serwer.

Symfony2 ma wbudowany mechanizm do przechowywania sesji w bazie danych 
:class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\PdoSessionStorage`.
Aby z niego skorzystać, wystarczy zmienić parametry w ``config.yml`` (lub
wybrany przez siebie format konfiguracji):

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            session:
                # ...
                storage_id:     session.storage.pdo

        parameters:
            pdo.db_options:
                db_table:    session
                db_id_col:   session_id
                db_data_col: session_value
                db_time_col: session_time

        services:
            pdo:
                class: PDO
                arguments:
                    dsn:      "mysql:dbname=mydatabase"
                    user:     myuser
                    password: mypassword

            session.storage.pdo:
                class:     Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage
                arguments: [@pdo, %session.storage.options%, %pdo.db_options%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:session storage-id="session.storage.pdo" default-locale="en" lifetime="3600" auto-start="true"/>
        </framework:config>

        <parameters>
            <parameter key="pdo.db_options" type="collection">
                <parameter key="db_table">session</parameter>
                <parameter key="db_id_col">session_id</parameter>
                <parameter key="db_data_col">session_value</parameter>
                <parameter key="db_time_col">session_time</parameter>
            </parameter>
        </parameters>

        <services>
            <service id="pdo" class="PDO">
                <argument>mysql:dbname=mydatabase</argument>
                <argument>myuser</argument>
                <argument>mypassword</argument>
            </service>

            <service id="session.storage.pdo" class="Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage">
                <argument type="service" id="pdo" />
                <argument>%session.storage.options%</argument>
                <argument>%pdo.db_options%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.yml
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        $container->loadFromExtension('framework', array(
            // ...
            'session' => array(
                // ...
                'storage_id' => 'session.storage.pdo',
            ),
        ));

        $container->setParameter('pdo.db_options', array(
            'db_table'      => 'session',
            'db_id_col'     => 'session_id',
            'db_data_col'   => 'session_value',
            'db_time_col'   => 'session_time',
        ));

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=mydatabase',
            'myuser',
            'mypassword',
        ));
        $container->setDefinition('pdo', $pdoDefinition);

        $storageDefinition = new Definition('Symfony\Component\HttpFoundation\SessionStorage\PdoSessionStorage', array(
            new Reference('pdo'),
            '%session.storage.options%',
            '%pdo.db_options%',
        ));
        $container->setDefinition('session.storage.pdo', $storageDefinition);

* ``db_table``: Nazwa tabeli sesji w bazie danych
* ``db_id_col``: Nazwa pola id w tabeli sesji (VARCHAR(255) lub większe)
* ``db_data_col``: Nazwa pola dla zapisywanych wartości w tabeli sesji (TEXT lub CLOB)
* ``db_time_col``: Nazwa pola dla czasu w tabeli sesji (INTEGER)

Udostępnianie informacji połączenia z bazą danych
-------------------------------------------------

W danej konfiguracji, ustawienie połączenia z bazą danych są określone dla
połączenia przechowywania sesji. To jest OK, gdy używasz oddzielnej
bazy danych dla danych sesyjnych.

Jeśli chcesz do przechowywania danych sesji użyć tej samej bazy danych, jak w reszcie
projektu, można użyć ustawienia połączenia z parameter.ini. Odwołanie do bazy danych
odbywa się przez zdefiniowane parametry:

.. configuration-block::

    .. code-block:: yaml

        pdo:
            class: PDO
            arguments:
                - "mysql:dbname=%database_name%"
                - %database_user%
                - %database_password%

    .. code-block:: xml

        <service id="pdo" class="PDO">
            <argument>mysql:dbname=%database_name%</argument>
            <argument>%database_user%</argument>
            <argument>%database_password%</argument>
        </service>

    .. code-block:: xml

        $pdoDefinition = new Definition('PDO', array(
            'mysql:dbname=%database_name%',
            '%database_user%',
            '%database_password%',
        ));

Przykład definicji dla MySQL
----------------------------

Instrukcja SQL (dla MySQL) tworzenia struktury tabeli może 
wyglądać następująco:

.. code-block:: sql

    CREATE TABLE `session` (
        `session_id` varchar(255) NOT NULL,
        `session_value` text NOT NULL,
        `session_time` int(11) NOT NULL,
        PRIMARY KEY (`session_id`),
        UNIQUE KEY `session_id_idx` (`session_id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;