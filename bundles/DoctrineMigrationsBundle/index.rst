.. highlight:: php
   :linenothreshold: 2

.. index::
   single: pakiety; DoctrineMigrationsBundle
   single: DoctrineMigrationsBundle


DoctrineMigrationsBundle
========================

Funkcjonalność migracji bazy danych jest rozszerzeniem warstwy abstrakcji bazy danych
i oferuje możliwość programowego wdrożenia nowych wersji schematu bazy danych w
bezpieczny, łatwy i znormalizowany sposób.

.. tip::

    Możesz dowiedzieć się więcej o migracji baz danych w Doctrine na stronach
    `dokumentacji`_ projektu.

Instalacja
----------

Migracje Doctrine dla Symfony są utrzymywane w pakiecie `DoctrineMigrationsBundle`_.
Pakiet ten wykorzystuje zewnętrzną bibliotekę `Doctrine Database Migrations`_.

Wykonaj następujące czynności aby zainstalować wyżej wspomniany pakiet i bibliotekę
w Symfony Standard Edition. W pliku ``composer.json`` umieść następujący kod:

.. code-block:: json
   :linenos:

    {
        "require": {
            "doctrine/doctrine-migrations-bundle": "dev-master"
        }
    }

Zaktualizuj biblioteki dostawców:

.. code-block:: bash

    $ php composer.phar update

Jeżeli wszystko przebiegło dobrze, to moża znależć pliki ``DoctrineMigrationsBundle``
w katalogu ``vendor/doctrine/doctrine-migrations-bundle``.

.. note::

    Pakiet ``DoctrineMigrationsBundle`` instaluje bibliotekę
    `Doctrine Database Migrations`_. Bibliotekę tą można znaleźć w katalogu
    ``vendor/doctrine/migrations``.

Na koniec należy pamiętać o włączeniu pakietu w ``AppKernel.php`` przez dodanie
następującego kodu:

.. code-block:: php
   :linenos:

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            //...
            new Doctrine\Bundle\MigrationsBundle\DoctrineMigrationsBundle(),
        );
    }

Stosowanie
----------

Wszystkie funkcjonalności migracji są zawarte w kilku poleceniach konsoli:

.. code-block:: bash

    doctrine:migrations
      :diff     Generate a migration by comparing your current database to your mapping information.
      :execute  Execute a single migration version up or down manually.
      :generate Generate a blank migration class.
      :migrate  Execute a migration to a specified version or the latest available version.
      :status   View the status of a set of migrations.
      :version  Manually add and delete migration versions from the version table.

Rozpocznij od pobrania statusu migracji w aplikacji, uruchamiając następujące
polecenie ``status``:

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuration

        >> Name:                                               Application Migrations
        >> Configuration Source:                               manually configured
        >> Version Table Name:                                 migration_versions
        >> Migrations Namespace:                               Application\Migrations
        >> Migrations Directory:                               /path/to/project/app/DoctrineMigrations
        >> Current Version:                                    0
        >> Latest Version:                                     0
        >> Executed Migrations:                                0
        >> Available Migrations:                               0
        >> New Migrations:                                     0

Teraz można rozpocząć pracę z migracjami, generując nową pustą klasę migracji.
Później dowiemy się jak Doctrine może automatycznie generować migracje.

.. code-block:: bash

    php app/console doctrine:migrations:generate
    Generated new migration class to "/path/to/project/app/DoctrineMigrations/Version20100621140655.php"

Spójrzmy na na nowo utworzoną klasę migracji::

    namespace Application\Migrations;

    use Doctrine\DBAL\Migrations\AbstractMigration,
        Doctrine\DBAL\Schema\Schema;

    class Version20100621140655 extends AbstractMigration
    {
        public function up(Schema $schema)
        {

        }

        public function down(Schema $schema)
        {

        }
    }

Jeżeli uruchomimy polecenie ``status``, to pokaże ono teraz, że mamy do wykonania
jedną migrację:

.. code-block:: bash

    php app/console doctrine:migrations:status

     == Configuration

       >> Name:                                               Application Migrations
       >> Configuration Source:                               manually configured
       >> Version Table Name:                                 migration_versions
       >> Migrations Namespace:                               Application\Migrations
       >> Migrations Directory:                               /path/to/project/app/DoctrineMigrations
       >> Current Version:                                    0
       >> Latest Version:                                     2010-06-21 14:06:55 (20100621140655)
       >> Executed Migrations:                                0
       >> Available Migrations:                               1
       >> New Migrations:                                     1

    == Migration Versions

       >> 2010-06-21 14:06:55 (20100621140655)                not migrated

Teraz można dodać trochę kodu migracji do metod ``up()`` i ``down()`` i w końcu
migrować, gdy się jest gotowy:

.. code-block:: bash

    php app/console doctrine:migrations:migrate

Aby uzyskać więcej informacji na temat pisania migracji (czyli jak wypełniać metody
``up()`` i ``down()``), zobacz od oficjalnej `dokumentacji`_ Doctrine Migrations.

Uruchamianie migracji podczas prac wdrożeniowych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Oczywiście celem końcowym pisania migracji jest zastosowanie ich do niezawodnej
aktualizacji struktury swojej bazy danych podczas wdrażania aplikacji.
Uruchamiając aplikacje lokalnie (lub na serwerze beta), można być pewnym,
że migracje działają właściwie.

Gdy już zakończy się wdrażanie aplikacji, wystarczy pamiętać, aby uruchomić polecenie
``doctrine:migrations:migrate``. Doctrine wewnętrznie utworzy w bazie danych tabelę
``migration_versions``  i prześledzi, które migracje zostały tam wykonane. Tak więc,
bez względu na to, ile migracji zostało stworzonych i wykonanych lokalnie, gdy
uruchomi się to polecenie w trakcie wdrażania, Doctrine będzie wiedzieć dokładnie
które migracje nie zostały jeszcze uruchomione poprzez wglądowi w tabelę
``migration_versions`` produkcyjnej bazy danych. Niezależnie od tego, jaki jest
serwer, zawsze można bezpiecznie uruchomić omawiane polecenie, aby wykonać tylko
migracje, które nie zostały jeszcze uruchomione na konkretnej bazie danych.

Automatyczne generowanie migracji
---------------------------------

W rzeczywistości, bardzo rzadko zachodzi konieczność ręcznego pisania migracji,
jako że biblioteka migracji może generować automatycznie klasy migracji porównując
informację odwzorowania Doctrine (czyli jak baza danych *powinna* wyglądać)
z obecną strukturą bazy danych.

Na przykład załóżmy, że tworzymy nową encję ``User`` i dodajemy informację
odwzorowania dla ORM Doctrine:

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/HelloBundle/Entity/User.php
        namespace Acme\HelloBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="hello_user")
         */
        class User
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length="255")
             */
            protected $name;
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/HelloBundle/Resources/config/doctrine/User.orm.yml
        Acme\HelloBundle\Entity\User:
            type: entity
            table: hello_user
            id:
                id:
                    type: integer
                    generator:
                        strategy: AUTO
            fields:
                name:
                    type: string
                    length: 255

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/HelloBundle/Resources/config/doctrine/User.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\HelloBundle\Entity\User" table="hello_user">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO"/>
                </id>
                <field name="name" column="name" type="string" length="255" />
            </entity>

        </doctrine-mapping>

Dzięki tej informacji Doctrine jest gotowy do pomocy w utrzymywaniu nowego obiektu
``User``, do i od tabeli ``hello_user``. Oczywiście ta tabela jeszcze nie istnieje.
Nową migrację dla tej tabeli generuje się automatycznie przez uruchomienie następującego
polecenia:

.. code-block:: bash

    php app/console doctrine:migrations:diff

Po wykonaniu tego polecenia powinien pojawić się komunikat, że nowa klasa migracji
została wygenerowana na podstawie różnicy schematów. Jeśli otworzysz ten plik, to
przekonasz się, że zawiera on kod SQL potrzebny do utworzenia tabeli ``hello_user``.

Następnie uruchomimy migrację, aby dodać tabelę do bazy danych:

.. code-block:: bash

    php app/console doctrine:migrations:migrate

Morał tej historii jest taki: po każdej zmianie można zrealizować informację odwzorowania
Doctrine, uruchamiając polecenie ``doctrine:migrations:diff`` w celu automatycznego
wygenerowania klas migracji.

Jeśli zrobi się to od samego początku projektu (tj. tak, że nawet pierwsze tabele
zostaną załadowane przy użyciu klasy migracji), będzie się w stanie zawsze utworzyć
świeżą bazę danych i uruchomić migracje w celu uzyskania w pełni aktualnego schematu
bazy danych. W rzeczywistości jest to łatwa i niezawodna organizacja pracy dla projektu.

Kontener świadomych migracji
----------------------------

W niektórych przypadkach zachodzi potrzeba dostępu do kontenera w celu zapewnienia
właściwej aktualizacji struktury danych. Może to być konieczne dla aktualizacji
relacji z jakimiś określonymi logikami lub dla utworzenia nowych encji.

W tym celu, aby uzyskać pełny dostęp do kontenera, można po prostu zaimplementować
interfejs ``ContainerAwareInterface`` z jego potrzebnymi metodami.

.. code-block:: php
   :linenos:

    // ...
    
    class Version20130326212938 extends AbstractMigration implements ContainerAwareInterface
    {
    
        private $container;
    
        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }
    
        public function up(Schema $schema)
        {
            // ... migration content
        }
    
        public function postUp(Schema $schema)
        {
            $em = $this->container->get('doctrine.orm.entity_manager');
            // ... update the entities
        }
    }

.. _`dokumentacji`: http://docs.doctrine-project.org/projects/doctrine-migrations/en/latest/index.html
.. _DoctrineMigrationsBundle: https://github.com/doctrine/DoctrineMigrationsBundle
.. _`Doctrine Database Migrations`: https://github.com/doctrine/migrations
