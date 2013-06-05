.. highlight:: php
   :linenothreshold: 2

.. index::
   single: Doctrine; DoctrineMongoDBBundle
   single: pakiety; DoctrineMongoDBBundle
   single: DoctrineMongoDBBundle


DoctrineMongoDBBundle
=====================

.. toctree::
   :hidden:

   config
   form

Biblioteka `MongoDB`_ Object Document Mapper (ODM) jest bardzo podobna
pod względem filozofii i sposobu działania do Doctrine2 ORM. Innymi słowami,
podobnie jak w :doc:`Doctrine2 ORM</book/doctrine>`, w Doctrine ODM ma się do czynienia tylko
ze zwykłymi obiektami PHP, które są następnie utrwalane w MongoDB.

.. tip::

    Możesz przeczytać więcej o Doctrine MongoDB ODM w `dokumentacji`_ tego projektu.

Pakiet jest dostępny w celu zintegrowania biblioteki Doctrine MongoDB ODM z Symfony,
czyniąc ją łatwą w konfiguracji i stosowaniu.

.. note::

    Rozdział ten jest bardzo podobny do :doc:`Doctrine2 ORM chapter</book/doctrine>`,
    w którym omawia się wykorzystanie Doctrine ORM do utrwalania danych w relacyjnych
    bazach danych (np. MySQL). Jest to celowe – niezależnie od tego, czy utrzymujemy
    dane w relacyjnej bazie danych z wykorzystaniem ORM, czy w MongoDB poprzez ODM,
    filozofia jest taka sama.

Instalacja
----------

Do używania MongoDB ODM potrzebne są dwie biblioteki udostępniane przez Doctrine
oraz pakiet, który je integruje z Symfony.

Instalacja pakietu przez Composer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aby zainstalować DoctrineMongoDBBundle przez Composer, wystarczy dodań następujący
kod do pliku `composer.json` file::

    {
        "require": {
            "doctrine/mongodb-odm": "1.0.*@dev",
            "doctrine/mongodb-odm-bundle": "3.0.*@dev"
        },
    }

Określenie ``@dev`` jest niezbędne dopóki nie będzie dostępna co wersja pakiet 
i ODM bardziej stabilana niż beta. Pozostałe oznaczenia wersji mogą być używane,
w zależności od potrzeb projektu, co omówione jest w `dokumentacji schematu`_ Composer.
Jak wyjaśniono w `dokumentacji schematu`_ Composer, użycie ``@dev`` powoduje
pobranie najbardziej stabilnej wersji pakietu, a pominięcie, pobranie wersji `stable`,
która może być niedostępna.  

Można teraz zainstalować nowe zależności przez uruchomienie polecenia ``update``
Composer w katalogu, w którym znajduje się plik  ``composer.json``:

.. code-block:: bash

    $ php composer.phar update

Teraz Composer automatycznie pobierze wszystkie wymagane pliki i je zainstaluje.

Zarejestrowanie adnotacji i pakietu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Następnie zarejestruj bibliotekę adnotacji, dodając do autoloadera następujący
kod (poniżej istniejącej linii ``AnnotationRegistry::registerLoader``)::

    // app/autoload.php
    use Doctrine\ODM\MongoDB\Mapping\Driver\AnnotationDriver;
    AnnotationDriver::registerAnnotationClasses();

Wszystko co pozostało do zrobienia, to tylko zaktualizowanie pliku ``AppKernel.php``
i zarejestrowanie nowego pakietu::

    // app/AppKernel.php
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Doctrine\Bundle\MongoDBBundle\DoctrineMongoDBBundle(),
        );

        // ...
    }

Zrobione! Jesteśmy gotowi do pracy.

Konfiguracja
------------

Aby rozpocząć, trzeba jeszcze wykonac podstawową konfigurację, która ustawia
menadżera dokumentów. Najprostszym sposobem jest włączenie opcji ``auto_mapping``,
która będzie aktywować w aplikacji MongoDB ODM:

.. code-block:: yaml
   :linenos:

    # app/config/config.yml
    doctrine_mongodb:
        connections:
            default:
                server: mongodb://localhost:27017
                options: {}
        default_database: test_database
        document_managers:
            default:
                auto_mapping: true

.. note::

    Oczywiście trzeba jeszcze upewnić się, że w tle działa serwer MongoDB.
    Więcej szczegółów o MongoDB można uzyskać w poradniku `Quick Start`_.

Prosty przykład: "Produkty"
---------------------------

Najlepszym sposobem na zrozumienie Doctrine MongoDB ODM jest zobaczenie tej bibliteki
w działaniu. W tym rozdziale przejdziemy przez całą procedurę potrzebną do utrzymania
dokumentów w MongoDB.

.. sidebar:: Ćwiczenie przykładowego kodu

    Jeśli chcesz przećwiczyć przykład z tego rozdziału, to utwórz pakiet
    ``AcmeStoreBundle`` poleceniem:

    .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/StoreBundle

Utworzenie klasy dokumentu
~~~~~~~~~~~~~~~~~~~~~~~~~~

Załóżmy, że budujemy aplikację w której powinny być wyświetlane produkty. Nawet
bez myślenia o Doctrine lub bazach danych wiesz już, że do reprezentowania produktów
potrzebny jest obiekt ``Product``. Utworzymy taką klasę wewnątrz katalogu ``Document``
w ``AcmeStoreBundle``::

    // src/Acme/StoreBundle/Document/Product.php
    namespace Acme\StoreBundle\Document;

    class Product
    {
        protected $name;

        protected $price;
    }

Klasa ta, często nazywana "dokumentem" (co oznacza *podstawową klasę przechowującą dane*),
jest prosta i pomaga spełnić w aplikacji wymóg procesów biznesowy potrzebujących produktów.
Klasa ta nie może jeszcze być utrwalona w Doctrine MongoDB – jest to tylko prosta klasa PHP.

Dodanie informacji odwzorowania
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine umożliwia pracę z MongoDB w sposób o wiele bardziej interesujący niż
tylko pobieranie danych tam i z powrotem jako tablicy. Zamiast tego, Doctrine
umożliwia utrzymywanie *obiektów* w MongoDB i pobieranie całych obiektów z MongoDB.
Działa to dzięki odwzorowaniu klasy PHP class i jej właściwości na kolekcję wpisów
MongoDB.

Aby to wykonać w Doctrine trzeba utworzyć "metadane" lub w konfiguracji ustawić
odwzorowanie klasy Product i jej właściwości na bazę danych. Metadane można określić
w kilku różnych formatach, włączając w to YAML, XML lub bezpośrednio w klasie
``Product`` poprzez adnotacje:

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/StoreBundle/Document/Product.php
        namespace Acme\StoreBundle\Document;

        use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;

        /**
         * @MongoDB\Document
         */
        class Product
        {
            /**
             * @MongoDB\Id
             */
            protected $id;

            /**
             * @MongoDB\String
             */
            protected $name;

            /**
             * @MongoDB\Float
             */
            protected $price;
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.yml
        Acme\StoreBundle\Document\Product:
            fields:
                id:
                    id:  true
                name:
                    type: string
                price:
                    type: float

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.xml -->
        <doctrine-mongo-mapping xmlns="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping
                            http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping.xsd">

            <document name="Acme\StoreBundle\Document\Product">
                <field fieldName="id" id="true" />
                <field fieldName="name" type="string" />
                <field fieldName="price" type="float" />
            </document>
        </doctrine-mongo-mapping>

Doctrine udostępnia wybór z szerokiej gamy różnych typów pól, każdy ze swoimi opcjami.
Więcej informacji o dostępnych typach pól można znaleźć w rozdziale :ref:`cookbook-mongodb-field-types`.

.. seealso::

    Możesz również sprawdzić `dokumentację Basic Mapping`_ Doctine w celu poznania
    bardziej szczegółowej informacji o odwzorowaniach. Jeśli używasz adnotacji,
    to musisz poprzedzać wszystkie adnotacje przedrostkiem ``MongoDB\``
    (np. ``MongoDB\String``), co nie jest pokazane w dokumentacji Doctrine.
    Potrzeba również dołączyć wyrażenie
    ``use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;``, które *zaimportuje*
    przedrostek adnotacji ``MongoDB``.

Wygenerowanie metod akcesorów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chociaż już Doctrine wie jak utrwalić obiekt ``Product`` w bazie danych, sama klasa
nie jest jeszcze przydatna. Ponieważ ``Product`` jest zwykłą klasą PHP, to potrzeba
utworzyć metody akcesorów pobierających i ustawiających (*ang. getter i setter*)
(tj. ``getName()`` i ``setName()``) w celu uzyskania dostępu do właściwości tego
obiektu (gdyż właściwości te są chronione). Doctrine może utworzyć te akcesory
w wyniku polecenia:

.. code-block:: bash

    $ php app/console doctrine:mongodb:generate:documents AcmeStoreBundle

Zastosowanie tego polecenia daje pewność, że w klasie ``Product`` zostaną wygenerowane
wszystkie niezbędne akcesory. Polecenie to jest bezpieczne – można uruchamiać je
w kółko - wygeneruje ono tylko nie istniejące akcesory (czyli nie nadpisuje istniejących
metod).

.. note::

    Dla Doctrine jest wszystko jedno, czy właściwości są chronione czy prywatne,
    lub czy istnieją akcesory dla właściwości. Akcesory są generowane tylko dlatego,
    że potrzebna jest interakcja z obiektem PHP.

Utrwalanie obiektów w MongoDB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Mamy już teraz całkowicie odwzorowany dokument ``Product`` i gotowe metody akcesorów
pobierających i ustawiających, jesteśmy więc gotowi do utrwalania danych w MongoDB.
Kod kontrolera jest całkiem prosty. Dodamy następującą metodę  do ``DefaultController``
pakietu:

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Document\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice('19.99');

        $dm = $this->get('doctrine_mongodb')->getManager();
        $dm->persist($product);
        $dm->flush();

        return new Response('Created product id '.$product->getId());
    }

.. note::

    Jeśli wykonujesz nasz przykład, to aby zobaczyć jak to działa musisz utworzyć
    trasę wskazującą na tą akcję.

Spójrzmy na powyższy kod bardziej szczegółowo:

* **linie 8-10** W tej sekcji tworzymy instancję klasy i działamy z obiektem ``$product``
  jak z innym zwykłym obiektem PHP;

* **linia 12** W tej linii pobieramy obiekt *menadżera dokumentów* Doctrine, który
  jest odpowiedzialny za obsługe procesu utrwalania i pobierania obiektów do i z MongoDB;

* **linia 13** Metoda ``persist()`` powiadamia Doctrine aby "zarządzała" obiektem
  ``$product``. W rzeczywistości to nie powoduje wprowadzenia zapytania do MongoDB
  (na razie);

* **linia 14** Gdy wywoływana jest metoda ``flush()``, Doctrine przeszukuje wszystkie
  obiekty, którymi zarządza, aby zobaczyć, czy potrzeba je utrwalić w MongoDB.
  W tym przykładzie obiekt ``$product`` nie został jeszcze utrwalony, więc menadżer
  dokumentów wykonuje zapytanie do MongoDB, które dodaje nowy wpis.

.. note::

    W rzeczywistości, ponieważ Doctrine ma informacje o wszystkich zarządzanych
    obiektach, to gdy wywoła się metodę ``flush()``, przeliczy ona całkowity wskaźnik
    zmian i wykona możliwie najlepsze zapytanie.

Podczas tworzenia lub aktualizowania obiektów, działanie jest zawsze takie samo.
W następnym  rozdziale zobaczymy jak Doctrine jest wystarczająco inteligentna aby
zaktualizować wpisy, gdy już istnieją w MongoDB.

.. tip::

    Doctrine dostarcza bibliotekę pozwalającą programowo załadować dane testowe
    do projektu. Więcej informacji uzyskasz w :doc:`/bundles/DoctrineFixturesBundle/index`.

Pobieranie obiektów z MongoDB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pobieranie z powrotem obiektów z bazy danych jest jeszcze bardziej łatwiejsze.
Na przykład, załóżmy, że skonfigurowana została trasa do wyświetlania konkretnego
produktu na podstawie jego wartości ``id``::

    public function showAction($id)
    {
        $product = $this->get('doctrine_mongodb')
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        // do something, like pass the $product object into a template
    }

Gdy przesyła się zapytanie dotyczące określonego typu obiektu, zawsze używa się czegoś,
co nazywa się "repozytorium". Można myśleć o repozytorium jak o klasie PHP, której
jedynym zadaniem jest pomoc w pobieraniu obiektów pewnych klas. Można uzyskać dostęp do
obiektu repozytorium dla klasy dokumentu poprzez::


    $repository = $this->get('doctrine_mongodb')
        ->getManager()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    Łańcuch ``AcmeStoreBundle:Product`` jest skrótem, jaki można używać zawsze
    w Doctrine zamiast pełnej nazwy dikumentu (tj. ``Acme\StoreBundle\Docement\Product``).
    Będzie to działać dopóty ważny jest dokument w przestrzeni nazw ``Document`` pakietu.

Po utworzeniu repozytorium ma się dostęp do wszelkiego rodzaju przydatnych metod::

    // query by the primary key (usually "id")
    $product = $repository->find($id);

    // dynamic method names to find based on a column value
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // find *all* products
    $products = $repository->findAll();

    // find a group of products based on an abitrary column value
    $products = $repository->findByPrice(19.99);

.. note::

    Oczywiście można również zadawać bardziej złożone zapytania o których można
    dowiedzieć się więcej w rozdziale :ref:`book-doctrine-queries`.

Można również wykorzystać przydatne metody ``findBy`` i ``findOneBy`` do łatwego
pobierania obiektu na podstawie różnych warunków::

    // query for one product matching be name and price
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // query for all prdocuts matching the name, ordered by price
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price', 'ASC')
    );

Aktualizowanie obiektu
~~~~~~~~~~~~~~~~~~~~~~

Po pobraniu obiektu z Doctrine, jego aktualizacja jest prosta. Załóżmy, że mamy
trasę, która odwzorowuje ``id`` produktu do kontrolera w celu przeprowadzenia
aktualizacji danych::

    public function updateAction($id)
    {
        $dm = $this->get('doctrine_mongodb')->getManager();
        $product = $dm->getRepository('AcmeStoreBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        $product->setName('New product name!');
        $dm->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

Aktualizacja obiektu obejmuje tylko trzy kroki:

#. pobranie obiektu przez Doctrine;
#. zmodyfikowanie obiektu;
#. wywołanie metody ``flush()`` w menadżerze dokumentów.

Proszę zauważyć, że wywołanie ``$dm->persist($product)`` nie jest konieczne.
Przypominamy, że metoda ta jedynie informuje Doctrine, aby zarządzało lub
"przyglądało się" obiektowi ``$product``. W naszym przypadku, ponieważ obiekt
``$product`` został już pobrany przez Doctrine, jest już on zarządzany.

Usuwanie obiektu
~~~~~~~~~~~~~~~~

Usuwanie obiektu jet bardzo podobne, ale wymaga wywołania metody ``remove()``
menadżera dokumentów::

    $dm->remove($product);
    $dm->flush();

Jak można się spodziewać, metoda ``remove()`` powiadamia Doctrine, że chce usunąć
określony dokument z MongoDB. Rzeczywista operacja usuwania nie jest faktycznie
przeprowadzana, do czasu wywołania metody``flush()``.

Zapytania do obiektów
---------------------

Jak widzieliśmy to poprzednio, wbudowana klasa repozytorium umożliwia wykonywanie
zapytań dla jednego lub wielu obiektów, na podstawie wielu różnych parametrów.
Gdy jest to wystarczające, jest to najprostszy sposób wykonywania zapytań dla dokumentów.
Oczywiście można tworzyć też bardziej skomplikowane zapytania.

Stosowanie konstruktora zapytań
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Biblioteka ODM Doctrine dostarczana jest z obiektem ``QueryBuilder``, który umożliwia
konstruować zapytania o dokładnie jakie dokumenty chce się otrzymać z bazy danych.
Jeżeli używa się IDE, to można również skorzystać z autouzupełniania podczas wpisywania
nazw metod. Z poziomu kontrolera::

    $products = $this->get('doctrine_mongodb')
        ->getManager()
        ->createQueryBuilder('AcmeStoreBundle:Product')
        ->field('name')->equals('foo')
        ->limit(10)
        ->sort('price', 'ASC')
        ->getQuery()
        ->execute()

W tym przypadku zwrócone zostanie 10 produktów o nazwie "foo", posortowanych od
najniżej do najwyższej ceny.

Obiekt ``QueryBuilder`` posiada wszystkie metody niezbędne do budowania zapytań.
Więcej informacji o Query Builder Doctrine można znaleźć w dokumentacji
`Query Builder`_ Doctrine. Aby uzyskać listę dostępnych warunków jakie można
umieścić w zapytaniu, zobacz odrębną dokumentację `operatorów warunkowych`_.

Niestandardowe klasy repozytorium
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W poprzednim rozdziale zaczęliśmy konstruować i stosować bardziej złożone zapytania
wewnątrz kontrolera. W celu izolacji, testowanie i ponowne używanie tych zapytań,
dobrym pomysłem jest utworzenie własnej klasy repozytorium dla dokumentów i dodanie
tam metody z logiki swojego zapytania.

Żeby to zrobić trzeba dodać nazwę klasy repozytorium do definicji odwzorowań.

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/StoreBundle/Document/Product.php
        namespace Acme\StoreBundle\Document;

        use Doctrine\ODM\MongoDB\Mapping\Annotations as MongoDB;

        /**
         * @MongoDB\Document(repositoryClass="Acme\StoreBundle\Repository\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.yml
        Acme\StoreBundle\Document\Product:
            repositoryClass: Acme\StoreBundle\Repository\ProductRepository
            # ...

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.mongodb.xml -->
        <!-- ... -->
        <doctrine-mongo-mapping xmlns="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping
                            http://doctrine-project.org/schemas/odm/doctrine-mongo-mapping.xsd">

            <document name="Acme\StoreBundle\Document\Product"
                    repository-class="Acme\StoreBundle\Repository\ProductRepository">
                <!-- ... -->
            </document>

        </doctrine-mong-mapping>

Doctrine wygeneruje klasę repozytorium po uruchomieniu polecenia:

.. code-block:: bash

    $ php app/console doctrine:mongodb:generate:repositories AcmeStoreBundle

Następnie trzeba dodać nową metodę ``findAllOrderedByName()`` do nowo wygenerowanej
klasy repozytorium. Metoda ta będzie wykonywać zapytania dla wszystkich dokumentów
``Product``, posortowanych alfabetycznie.

.. code-block:: php
   :linenos:

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    namespace Acme\StoreBundle\Repository;

    use Doctrine\ODM\MongoDB\DocumentRepository;

    class ProductRepository extends DocumentRepository
    {
        public function findAllOrderedByName()
        {
            return $this->createQueryBuilder()
                ->sort('name', 'ASC')
                ->getQuery()
                ->execute();
        }
    }

Możesz wykorzystywać tą nową metodę , podobnie ja domyślne metody findera repozytorium::

    $products = $this->get('doctrine_mongodb')
        ->getManager()
        ->getRepository('AcmeStoreBundle:Product')
        ->findAllOrderedByName();


.. note::

    Gdy wykorzystuje się własną klasę repozytorium, to nadal ma się dostęp do
    domyślnych metod findera, takich jak ``find()`` czy ``findAll()``.

Rozszerzenia Doctrine: Timestampable, Sluggable itd.
----------------------------------------------------

Doctrine jest dość elastyczna i dostępnych jest kilka rozszerzeń osób trzecich,
które pozwalają łatwo wykonywać powtarzające się i popularne zadania na dokumentach.
Dotyczy to takich rzeczy jak *Sluggable*, *Timestampable*, *Loggable*, *Translatable*
i *Tree*.

Aby uzyskać informację o tym, jak znaleźć i używać te rozszerzenia można znaleźć
w artykule ":doc:`/cookbook/doctrine/common_extensions`".

.. _cookbook-mongodb-field-types:

Informacje o typach pól Doctrine
--------------------------------

Doctrine dostarczana jest z dużą ilością typów pól. Każdy z nich odwzorowuje typ
danych PHP na odpowiednie `typy MongoDB`_. Oto tylko niektóre typy obsługiwane
w Doctrine:

* ``string``
* ``int``
* ``float``
* ``date``
* ``timestamp``
* ``boolean``
* ``file``

Więcej informacji można znaleźć w `dokumentacji typów odwzorowania`_ Doctrine.

.. index::
   single: Doctrine; polecenia konsoli ODM
   single: CLI; Doctrine ODM

Polecenia konsoli
-----------------

Mechanizm integracji ODM Doctrine2 z Symfony2 udostępnia kilka poleceń konsoli
w przestrzeni nazw ``doctrine:mongodb``. Aby wyświetlić listę poleceń, trzeba
uruchomić polecenie ``app/console`` bez jakichkolwiek argumentów:

.. code-block:: bash

    $ php app/console

Polecenie to wydrukuje listę dostępnych poleceń, z których wiele rozpoczyna się
przedrostkiem ``doctrine:mongodb``. Można znaleźć wiele informacji o tych poleceniach
(lub innych poleceniach Symfony) przez uruchomienie polecenia ``help``. Na przykład,
aby uzyskać szczegóły o zadaniu ``doctrine:mongodb:query`` uruchom:

.. code-block:: bash

    $ php app/console help doctrine:mongodb:query

.. note::

   Dla załadowania danych testowych do MongoDB należy zainstalować pakiet
   ``DoctrineFixturesBundle``. Więcej na ten temat dowiesz się w dokumencie
   ":doc:`/bundles/DoctrineFixturesBundle/index`".

.. index::
   single: konfiguracja; Doctrine MongoDB ODM
   single: Doctrine; konfiguracja MongoDB ODM

Konfiguracja
------------

Szczegółowe informacje o opcjach konfiguracyjnych Doctrine ODM omówione są w rozdziale
":doc:`/bundles/DoctrineMongoDBBundle/config`".

Rejestrowanie detektorów zdarzeń i subskrybentów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine umożliwia zarejestrowanie detektorów nasłuchujących (*ang. listners*)
i subskrybentów (*ang. subscribers*), które są zgłaszane, gdy w ODM Doctrine
wystąpią różne zdarzenia. Więcej informacji znajduje się w `dokumentacji zdarzeń`_ Doctrine.

.. tip::

    Oprócz zdarzeń ODM, można również nasłuchiwac zdarzenia wyższego poziomu MongoDB,
    które są zdefiniowane w klasie ``Doctrine\MongoDB\Events``.

.. note::

    Każde połączenie w Doctrine ma swojego własnego menadżera zdarzeń, który jest
    współdzielony z menadżerami dokumentów związanymi z tym połączeniem.
    Detektory nasłuchujące i subskrybenty mogą zostać zarejestrowane ze wszystkimi
    menadżerami zdarzeń lub tylko z jednym (przy użyciu nazwy połączenia).

W Symfony można zarejestrować detektor i subskrybenta przez utworzenie :term:`usługi <usługa>`
a później określone :ref:`znacznika <book-service-container-tags>`.

*   **detektor zdarzeń**: Do zarejestrowania detektora stosuje się znacznik
    ``doctrine_mongodb.odm.event_listener``. Atrybut ``event`` jest wymagany i powinien oznaczać
    zdarzenie do nasłuchiwania. Domyślnie detektory zostaną zarejestrowane z menadżerami
    zdarzenia dla wszystkich połączeń. Aby ograniczyć detektor do jednego połączenia,
    należy określić jego nazwę w atrybucie ``connection`` znacznika.

    Atrybut ``priority``, którego domyślna wartość wynosi ``0`` gdy jest pominięty,
    może być wykorzystany do ustalania kolejności, w jakiej są rejestrowane detektory.
    Podobnie jak w :doc:`dyspozytorze zdarzeń </components/event_dispatcher/introduction>`
    Symfony2, detektor o większym priorytecie będzie wykonywany pierwszy a detektory
    o tym samym priorytecie beda wykonywane w kolejności ich zarejestrowania
    w menadżerze zdarzeń.

    Nieobowiązkowy atrybut ``lazy``, którego domyślna wartość wynosi ``false``,
    może być użyty dla żądania, którego detektor będzie ładowany „leniwie” przez
    menadżera zdarzenia, gdy jego zdarzenie jest wysyłane.

    .. configuration-block::

        .. code-block:: yaml
           :linenos:

            services:
                my_doctrine_listener:
                    class:   Acme\HelloBundle\Listener\MyDoctrineListener
                    # ...
                    tags:
                        -  { name: doctrine_mongodb.odm.event_listener, event: postPersist }

        .. code-block:: xml

            <service id="my_doctrine_listener" class="Acme\HelloBundle\Listener\MyDoctrineListener">
                <!-- ... -->
                <tag name="doctrine_mongodb.odm.event_listener" event="postPersist" />
            </service>.

        .. code-block:: php

            $definition = new Definition('Acme\HelloBundle\Listener\MyDoctrineListener');
            // ...
            $definition->addTag('doctrine_mongodb.odm.event_listener', array(
                'event' => 'postPersist',
            ));
            $container->setDefinition('my_doctrine_listener', $definition);

*   **subskrybent zdarzeń**: Do zarejestrowania subskrybenta stosuje się znacznik
    ``doctrine_mongodb.odm.event_subscriber``. Subskrybenty są odpowiedzialne za
    implementację ``Doctrine\Common\EventSubscriber`` oraz metodę do zwracania zdarzeń,
    które będą obserwowane. Z tego powodu znacznik ten nie ma atrybutu ``event``, ale
    dostępne są atrybuty ``connection``, ``priority`` i ``lazy``.

.. note::

    W przeciwieństwie do detektora zdarzeń Symfony2, menadżer zdarzeń Doctrine
    oczekuje, że każdy detektor i subskrybent ma nazwę metody odpowiadającej
    obserwowanemu zdarzeniu (zdarzeniom). Dlatego wspomniane znaczniki nie mają
    atrybutu ``method``.

Integracja z SecurityBundle
---------------------------

W projektach MongoDB dostępny jest dostawca uzytkowników, działający dokładnie
tak samo jak dostawca `encji` opisany w artykule ":doc:`/cookbook/security/entity_provider`".

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        security:
            providers:
                my_mongo_provider:
                    mongodb: {class: Acme\DemoBundle\Document\User, property: username}

    .. code-block:: xml
       :linenos:

        <!-- app/config/security.xml -->
        <config>
            <provider name="my_mongo_provider">
                <mongodb class="Acme\DemoBundle\Document\User" property="username" />
            </provider>
        </config>

Podsumowanie
------------

Używając Doctrine, można skupić się przede wszystkim na swoich obiektach i na tym
jak są one przydatne w aplikacji i nie zaprzątać sobie głowy problemami utrzymania
danych w MongoDB. Dzieje się tak dlatego, że Doctrine umożliwia stosowanie
jakiegokolwiek obiektu o przechowywania danych i korzysta z informacji metadanych
odwzorowań do odwzorowywania danych obiektu na kolekcję MongoDB.

Pomimo, że Doctrine działa zgodnie z prostą koncepcja, jest bardzo zaawansowaną
biblioteką, umożliwiającą tworzenie złożonych zapytań i subskrybowanie zdarzeń,
pozwalając na w pełni obiektowe podejście.

Dalsza lektura
--------------

* :doc:`/bundles/DoctrineMongoDBBundle/form`

.. _`MongoDB`:          http://www.mongodb.org/
.. _`dokumentacji`:    http://docs.doctrine-project.org/projects/doctrine-mongodb-odm/en/latest/
.. _`dokumentacji schematu`: http://getcomposer.org/doc/04-schema.md#minimum-stability
.. _`Quick Start`:      http://www.mongodb.org/display/DOCS/Quickstart
.. _`dokumentację Basic Mapping`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/basic-mapping.html
.. _`typy MongoDB`: http://us.php.net/manual/en/mongo.types.php
.. _`dokumentacji typów odwzorowania`: http://docs.doctrine-project.org/projects/doctrine-mongodb-odm/en/latest/reference/basic-mapping.html#doctrine-mapping-types
.. _`Query Builder`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/query-builder-api.html
.. _`operatorów warunkowych`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/query-builder-api.html#conditional-operators
.. _`dokumentacji zdarzeń`: http://www.doctrine-project.org/docs/mongodb_odm/1.0/en/reference/events.html
