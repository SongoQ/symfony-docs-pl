.. highlight:: php
   :linenothreshold: 2

.. index::
   pair: Doctrine; DBAL

Jak używać warstwy DBAL z Doctrine
==================================

.. note::

    Artykuł ten traktuje o warstwie Doctrine DBAL. Zazwyczaj będziesz pracował z
    warstwą wyższego poziomu - Doctrine ORM, która wykorzystuje w tle DBAL do
    rzeczywistego komunikowania się z bazą danych. Aby dowiedzieć się więcej o
    Doctrine ORM, proszę przeczytać ":doc:`/book/doctrine`".

`Doctrine`_ Database Abstraction Layer (DBAL) jest abstrakcyjną warstwą ulokowana
na szczycie `PDO`_ i dostarczajaca intuicyjne i elastyczne API dla komunikacji z
najbardziej popularnymi relacyjnymi bazami danych. Innymi słowami, biblioteka DBAL
ułatwia wykonywanie zapytań i realizację innych działań na bazie danych.

.. tip::

    Przeczytaj oficjalną `dokumentację DBAL`_ w celu poznania wszystkich szczegółów
    i możliwości jakie daje biblioteka DBAL Doctrine.

Aby rozpocząć należy skonfigurować parametry połączenia z bazą danych:


.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   pdo_mysql
                dbname:   Symfony2
                user:     root
                password: null
                charset:  UTF8

    .. code-block:: xml
       :linenos:

        // app/config/config.xml
        <doctrine:config>
            <doctrine:dbal
                name="default"
                dbname="Symfony2"
                user="root"
                password="null"
                driver="pdo_mysql"
            />
        </doctrine:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'    => 'pdo_mysql',
                'dbname'    => 'Symfony2',
                'user'      => 'root',
                'password'  => null,
            ),
        ));

W celu poznania wszystkich opcji konfiguracyjnych DBAL proszę zobaczyć
:ref:`reference-dbal-configuration`.

Można uzyskać dostęp do połączenia Doctrine DBAL poprzez usługę ``database_connection``::

    class UserController extends Controller
    {
        public function indexAction()
        {
            $conn = $this->get('database_connection');
            $users = $conn->fetchAll('SELECT * FROM users');

            // ...
        }
    }


Rejestracja niestandardowych typów odwzorowań
---------------------------------------------

Zarejestrowanie niestandardowych typów odwzorowań dokonuje się w konfiguracji Symfony.
Tak zarejestrowane typy zostaną dodane do wszystkich skonfigurowanych połączeń.
Więcej informacji o niestandardowych typach odwzorowań można znaleźć w rozdziale
`Niestandardowe typy odwzorowań`_ w dokumentacji Doctrine.


.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        doctrine:
            dbal:
                types:
                    custom_first: Acme\HelloBundle\Type\CustomFirst
                    custom_second: Acme\HelloBundle\Type\CustomSecond

    .. code-block:: xml
       :linenos:
       
        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                    <doctrine:type name="custom_first" class="Acme\HelloBundle\Type\CustomFirst" />
                    <doctrine:type name="custom_second" class="Acme\HelloBundle\Type\CustomSecond" />
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'types' => array(
                    'custom_first'  => 'Acme\HelloBundle\Type\CustomFirst',
                    'custom_second' => 'Acme\HelloBundle\Type\CustomSecond',
                ),
            ),
        ));

Rejestrowanie niestandardowych typów odwzorowań w SchemaTool
------------------------------------------------------------

SchemaTool jest używany do kontroli porównawczej bazy danych ze schematem.
Do realizacji tego zadania trzeba znać typ potrzebnego odwzorowania, jaki należy
zastosować dla każdego typu bazy danych. Zarejestrowanie nowego typu można dokonać
za pomocą konfiguracji.

Odwzorujmy typ ENUM (nie obsługiwany domyślnie przez DBAL) na typ odwzorowania ``string``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        doctrine:
            dbal:
                connections:
                    default:
                        // Other connections parameters
                        mapping_types:
                            enum: string

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:dbal>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection>
                        <doctrine:mapping-type name="enum">string</doctrine:mapping-type>
                    </doctrine:connection>
                </doctrine:dbal>
            </doctrine:config>
        </container>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'connections' => array(
                    'default' => array(
                        'mapping_types' => array(
                            'enum'  => 'string',
                        ),
                    ),
                ),
            ),
        ));


.. _`PDO`:           http://www.php.net/pdo
.. _`Doctrine`:      http://www.doctrine-project.org/projects/dbal/2.0/docs/en
.. _`dokumentację DBAL`: http://www.doctrine-project.org/projects/dbal/2.0/docs/en
.. _`Niestandardowe typy odwzorowań`: http://www.doctrine-project.org/docs/dbal/2.0/en/reference/types.html#custom-mapping-types
