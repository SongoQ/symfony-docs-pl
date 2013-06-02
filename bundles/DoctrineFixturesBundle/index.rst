.. highlight:: php
   :linenothreshold: 2

.. index::
   single: pakiety; DoctrineFixturesBundle
   single: DoctrineFixturesBundle

DoctrineFixturesBundle
======================

Konfiguratory testowania (*ang. fixtures*) są używane (w Doctrine) do ładowania
kontrolowanych zbiorów danych do bazy danych. Dane te mogą zostać wykorzystane do
testowania lub jako dane początkowe niezbędne do sprawnego uruchomienia aplikacji.
Symfony2 nie ma wbudowanego tego typu oprogramowania, ale Doctrine2 ma bibliotekę
pomagającą napisać takie konfiguratory dla Doctrine :doc:`ORM</book/doctrine>` lub
:doc:`ODM</bundles/DoctrineMongoDBBundle/index>`.

Instalacj i konfiguracja
------------------------

Konfiguratory testowania Doctrine dla Symfony są utrzymywane w `DoctrineFixturesBundle`_.
Pakiet ten używa zewnętrznej biblioteki `Doctrine Data Fixtures`_.

Wykonaj następujące czynności aby w Symfony Standard Edition zainstalować wyżej
wymieniony pakiet i bibliotekę . Do pliku ``composer.json`` dodaj:

.. code-block:: json
   :linenos:

    {
        "require": {
            "doctrine/doctrine-fixtures-bundle": "dev-master"
        }
    }

Zaktualizuj biblioteki dostawców:

.. code-block:: bash

    $ php composer.phar update

Jeżeli wszystko działa, to teraz można znaleźć ppliki pakietu ``DoctrineFixturesBundle``
w katalogu ``vendor/doctrine/doctrine-fixtures-bundle``.

.. note::

    ``DoctrineFixturesBundle`` instaluje bibliotekę `Doctrine Data Fixtures`_.
    Biblioteke ta można znaleźć w ``vendor/doctrine/data-fixtures``.

Na koniec zarejestruj pakiet ``DoctrineFixturesBundle`` w ``app/AppKernel.php``:

.. code-block:: php
   :linenos:

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle(),
            // ...
        );
        // ...
    }

Pisanie prostych konfiguratorów testowania
------------------------------------------

Konfiguratory testowania Doctrine2 są klasami PHP, które mogą tworzyć obiekty przypadków
testowych i utrzymywać je w bazie danych. Podobnie jak inne klasy w Symfony2,
konfiguratory testowania powinny byś umieszczane w pakietach aplikacji.

Dla pakietu znajdującego się w ``src/Acme/HelloBundle`` klasy konfiguratora powinny
znajdować się w ``src/Acme/HelloBundle/DataFixtures/ORM`` albo
``src/Acme/HelloBundle/DataFixtures/MongoDB`` odpowiednio  dla ORM i ODM.
W niniejszym dokumencie założono, że używasz ORM – ale konfiguratory testowania mogą
być dodawane równie łatwo w ODM.

Wyobraźmy sobie, że mamy klasę ``User`` i chcemy załadować wpis jednego ``User``:

.. code-block:: php
   :linenos:

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData implements FixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setPassword('test');

            $manager->persist($userAdmin);
            $manager->flush();
        }
    }

W Doctrine2 konfiguratory testowania są tylko obiektami, do których można interaktywnie
załadować dane ze swoimi encjami, tak jak to się zwykle robi. Umożliwia to utworzenie
dla aplikacji konfiguratorów testowych, dokładnie według potrzeb.

Najpoważniejszym ograniczeniem jest to, że nie można współdzielić obiektów pomiędzy
pomiędzy konfiguratorami testowymi. Później poznamy obejście tego ograniczenia.

Uruchamianie konfiguratorów testowania
--------------------------------------

Gdy już konfiguratory testowania zostaną napisane, to można je załadować z poziomu
konsoli używając polecenia ``doctrine:fixtures:load``:

.. code-block:: bash

    php app/console doctrine:fixtures:load

Jeżeli stosujesz ODM, użyj zamiast tego polecenia ``doctrine:mongodb:fixtures:load``:

.. code-block:: bash

    php app/console doctrine:mongodb:fixtures:load

Zadanie będzie polegać na wejściu do katalogu ``DataFixtures/ORM``
(lub ``DataFixtures/MongoDB`` dla ODM) każdego pakietu i wykonaniu każdej klasy
implementującej ``FixtureInterface``.

Obydwa polecenia mają kilka opcji:

* ``--fixtures=/path/to/fixture`` - Użyj tej opcji aby ręcznie określić katalog
  do którego powinny być załadowane klasy konfiguratorów;

* ``--append`` - Użyj tej flagi aby dołączyć dane do poprzednich danych, zamiast
  usuwać dane przed załadowaniem nowych (usuwanie danych jest zachowaniem domyślnym);

* ``--em=manager_name`` - Ręczne określenie menadżera encji do użycia dla ładowanych danych.

.. note::

   Jeśli stosuje się zadanie ``doctrine:mongodb:fixtures:load``, to w celu ręcznego
   określenie menadżera dokumentu należy zamienić opcję ``--em=`` na ``--dm=``.

Przykład pełnego użycia wszystkich opcji wygląda tak:

.. code-block:: bash

   php app/console doctrine:fixtures:load --fixtures=/path/to/fixture1 --fixtures=/path/to/fixture2 --append --em=foo_manager

Udostępnianie obiektów pomiędzy konfiguratorami testowania
----------------------------------------------------------

Pisanie podstawowych konfiguratorów jest proste. Ale co jeśli ma się wiele klas
konfiguratorów i chce się aby były dostępne dla odwoływania się do danych załadowanych
w innych klasach konfiguratorów? Dla przykładu, co jeślo załadujemy obiekt ``User``
w jednym konfiguratorze i następnie chcemy odwoływać do innego konfiguratora
w celu przypisania użytkownikowi jakiejś grupy?

Biblioteka konfiguratorów Doctrine obsługuje to bez problemów, pozwalają określić
kolejność, w jakiej ładowane są konfiguratory.

.. code-block:: php
   :linenos:

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php
    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData extends AbstractFixture implements OrderedFixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $userAdmin = new User();
            $userAdmin->setUsername('admin');
            $userAdmin->setPassword('test');

            $manager->persist($userAdmin);
            $manager->flush();

            $this->addReference('admin-user', $userAdmin);
        }

        /**
         * {@inheritDoc}
         */
        public function getOrder()
        {
            return 1; // the order in which fixtures will be loaded
        }
    }

Klasa konfiguratora implementuje teraz ``OrderedFixtureInterface``, który powiadamia
Doctrine, że chce się kontrolować kolejność konfiguratorów. Utwórzmy inną klasę
konfiguratora i zróbmy tak, aby była ona ładowana po ``LoadUserData`` zwrócenie
w ``getOrder`` wartości 2:

.. code-block:: php
   :linenos:

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadGroupData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\Group;

    class LoadGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $groupAdmin = new Group();
            $groupAdmin->setGroupName('admin');

            $manager->persist($groupAdmin);
            $manager->flush();

            $this->addReference('admin-group', $groupAdmin);
        }

        /**
         * {@inheritDoc}
         */
        public function getOrder()
        {
            return 2; // the order in which fixtures will be loaded
        }
    }

Obie klasy konfiguratorów rozszerzają ``AbstractFixture``, która umożliwia utworzenie
obiektów i następnie ustawienie ich jako odniesienia, tak że mogą być  być wykorzystane
później w innych konfiguratorach. Na przykład, do obiektów ``$userAdmin`` i ``$groupAdmin``
można się później odwoływać poprzez odniesienia ``admin-user`` i ``admin-group``:

.. code-block:: php
   :linenos:

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserGroupData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\AbstractFixture;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\HelloBundle\Entity\UserGroup;

    class LoadUserGroupData extends AbstractFixture implements OrderedFixtureInterface
    {
        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $userGroupAdmin = new UserGroup();
            $userGroupAdmin->setUser($this->getReference('admin-user'));
            $userGroupAdmin->setGroup($this->getReference('admin-group'));

            $manager->persist($userGroupAdmin);
            $manager->flush();
        }

        /**
         * {@inheritDoc}
         */
        public function getOrder()
        {
            return 3;
        }
    }

Konfiguratory będą teraz wykonywane w kolejności rosnącej ustalonej przez wartości
zwracane w metodach ``getOrder()``. Każdy obiekt, który jest ustawiany przez metodę
``setReference()`` może być dostępny poprzez ``getReference()`` w klasach konfiguratorów
o wyższym priorytecie kolejności.

Konfiguratory testowania umożliwiają tworzenie danych dowolnego typu poprzez zwykły
interfejs PHP dla tworzenia i utrzymywania obiektów. Sterując kolejnością konfiguratorów
i ustawieniem odniesień można w konfiguratorach obsłużyć wszystko.

Używanie kontenera w konfiguratorach testowania
-----------------------------------------------

W niektórych przypadkach potrzebuje się mieć dostęp do jakichś usług aby załadować
konfiguratory. W Symfony2 jest to naprawdę proste: kontener będzie wstrzykiwany
do wszystkich klas konfiguratorów implementujących interfejs
:class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface`.

Przepiszmy pierwszy konfigurator, tak aby kodował hasło przed jego zapisaniem
w bazie danych (bardzo dobra praktyka). Wykorzystamy do kodowania hasła fabrykę
kodowania, upewniając się, że jest ono kodowane w sposób używający komponent
bezpieczeństwa podczas sprawdzania hasła:

.. code-block:: php
   :linenos:

    // src/Acme/HelloBundle/DataFixtures/ORM/LoadUserData.php

    namespace Acme\HelloBundle\DataFixtures\ORM;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Symfony\Component\DependencyInjection\ContainerAwareInterface;
    use Symfony\Component\DependencyInjection\ContainerInterface;
    use Acme\HelloBundle\Entity\User;

    class LoadUserData implements FixtureInterface, ContainerAwareInterface
    {
        /**
         * @var ContainerInterface
         */
        private $container;

        /**
         * {@inheritDoc}
         */
        public function setContainer(ContainerInterface $container = null)
        {
            $this->container = $container;
        }

        /**
         * {@inheritDoc}
         */
        public function load(ObjectManager $manager)
        {
            $user = new User();
            $user->setUsername('admin');
            $user->setSalt(md5(uniqid()));

            $encoder = $this->container
                ->get('security.encoder_factory')
                ->getEncoder($user)
            ;
            $user->setPassword($encoder->encodePassword('secret', $user->getSalt()));

            $manager->persist($user);
            $manager->flush();
        }
    }

Jak widać, wszystko co trzeba zrobić, to dodanie interfejsu
:class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface` do klasy
konfiguratora i następnie utworzenie nowej metody
:method:`Symfony\\Component\\DependencyInjection\\ContainerInterface::setContainer`
implementującej ten interfejs. Zanim konfigurator będzie wykonany, Symfony wywoła
automatycznie metodę
:method:`Symfony\\Component\\DependencyInjection\\ContainerInterface::setContainer`.
Tak jak długo będzie przechowywany kontener jako właściwość klasy (co pokazano powyżej),
tak długo będzie się miało do niego dostęp w metodzie``load()``.

.. note::

    Jeśli nie chcesz implementować potrzebnej metody
    :method:`Symfony\\Component\\DependencyInjection\\ContainerInterface::setContainer`,
    to możesz rozszerzyć klasę konfiguratora przez klasę
    :class:`Symfony\\Component\\DependencyInjection\\ContainerAware`.

.. _DoctrineFixturesBundle: https://github.com/doctrine/DoctrineFixturesBundle
.. _`Doctrine Data Fixtures`: https://github.com/doctrine/data-fixtures
