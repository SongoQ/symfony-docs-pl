.. index::
   pair: Doctrine; DBAL

Jak używać Warstwy DBAL z Doctrine
================================

.. note::

    Ten artykuł jest o warstwie DBAL z Doctrine. Zazwyczaj, będziesz pracował 
    z warstwą ORM która jest wyższą warstwą w Doctrine, która używa warstwy DBAL
    jakby "za sceną" do komunikowania się z bazą danych. Aby dowiedzieć się więcej
    na temat ORM dostarczanego przez Doctrine, zobacz ":doc:`/book/doctrine`".

`Doctrine`_ Database Abstraction Layer (DBAL) jest abstrakcyjną warstwą która ulokowana jest
na szczycie `PDO`_, umożliwia nam ona intuicyjne oraz elastyczne API do komunikacji z 
większością popularnych relacyjnych baz danych. Innymi słowy, biblioteka DBAL
ułatwia wywoływanie zapytań oraz wykonywanie innych akcji bazy danych.

.. tip::

    Przeczytaj oficjalną dokumentację Doctrine `DBAL Documentation`_
    aby dowiedzieć się wszystkich szczegółów i możliwości jakie daje biblioteka DBAL.

Aby rozpocząć, skonfiguruj parametry połączenia do bazy:


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                driver:   pdo_mysql
                dbname:   Symfony2
                user:     root
                password: null
                charset:  UTF8

    .. code-block:: xml

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

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'    => 'pdo_mysql',
                'dbname'    => 'Symfony2',
                'user'      => 'root',
                'password'  => null,
            ),
        ));

Aby zobaczyć pełną konfigurację DBAL, zobacz :ref:`reference-dbal-configuration`.

Możesz uzyskać dostęp do połączenia DBAL poprzez dostęp do usługi ``database_connection``:

.. code-block:: php

    class UserController extends Controller
    {
        public function indexAction()
        {
            $conn = $this->get('database_connection');
            $users = $conn->fetchAll('SELECT * FROM users');

            // ...
        }
    }

Rejestracja Niestandardowych Typów Mapowań
------------------------------------------

Możesz zarejestrować niestandardowe typy mapować poprzez konfigurację Symfony.
Zostaną one dodane do wszystkich skonfigurowanych połączeń. 
Aby uzyskać więcej informacji o niestandardowych typach mapowań, przeczytaj dokumentację
`Custom Mapping Types`_.


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                types:
                    custom_first: Acme\HelloBundle\Type\CustomFirst
                    custom_second: Acme\HelloBundle\Type\CustomSecond

    .. code-block:: xml

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

Rejestrowanie Niestandardowych Typów Mapowań w SchemaTool
---------------------------------------------------------

SchemaTool jest używany do porównywania bazy danych z schematem.
Do realizacji tego zadania, mechanizm musi wiedzieć który typ mapowania
ma być użyty do którego typu w bazie danych. Rejestracja odbywa się poprzez 
plik konfiguracyjny. 

Zmapujmy teraz type ENUM (nie wspierany domyślnie przez DBAL) do typu ``string``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            dbal:
                connection:
                    default:
                        // Other connections parameters
                        mapping_types:
                            enum: string

    .. code-block:: xml

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

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'types' => array(
                    'custom_first'  => 'Acme\HelloBundle\Type\CustomFirst',
                    'custom_second' => 'Acme\HelloBundle\Type\CustomSecond',
                ),
            ),
        ));

.. _`PDO`:           http://www.php.net/pdo
.. _`Doctrine`:      http://www.doctrine-project.org/projects/dbal/2.0/docs/en
.. _`DBAL Documentation`: http://www.doctrine-project.org/projects/dbal/2.0/docs/en
.. _`Custom Mapping Types`: http://www.doctrine-project.org/docs/dbal/2.0/en/reference/types.html#custom-mapping-types
