.. highlight:: php
   :linenothreshold: 2

.. index::
   single: Doctrine

Bazy danych i Doctrine
======================

Jednym z najbardziej powszechnych i trudnych zadań każdej aplikacji jest utrwalanie
i czytanie informacji zapisanej w bazie danych. Na szczęście Symfony jest zintegrowana
z biblioteką PHP `Doctrine`_, której jedynym celem jest dostarczenie skutecznych
narzędzi do uczynienia tego zadania łatwym. W tym rozdziale zapoznasz się z podstawami
filozofii stojącej za Doctrine i zobaczysz jak można łatwo pracować z bazą danych.

.. note::

    Doctrine jest całkowicie niezależną i oddzielną biblioteką i jej stosowanie
    w Symfony jest opcjonalne. Ten rozdział traktuje o Doctrine ORM, bibliotece
    pozwalającej odwzorować obiekty w relacyjnej bazie danych (takiej jak *MySQL*,
    *PostgreSQL* czy *Microsoft SQL*). Jeżeli wolisz używać surowych zapytań,
    to proste – zapoznaj się z artykułem ":doc:`/cookbook/doctrine/dbal`".

    Można również utrwalić dane w `MongoDB`_ stosując bibliotekę Doctrine ODM.
    Więcej informacji na ten temat znajdziesz w dokumentacji
    ":doc:`/bundles/DoctrineMongoDBBundle/index`".

Prosty przykład: "Produkty"
---------------------------

Najprostszym sposobem zrozumienia działania Doctrine jest zobaczenia jej w działaniu.
W tym rozdziale skonfigurujemy bazę danych, utworzymy obiekt ``Product``, zachowamy
go w bazie danych i pobierzemy go z powrotem.

.. sidebar:: Kod z przykładem

    Jeśli chcesz przećwiczyć przykład z tego rozdziału, to utwórz pakiet
    ``AcmeStoreBundle`` poleceniem:
    

    .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/StoreBundle

Konfiguracja bazy danych
~~~~~~~~~~~~~~~~~~~~~~~~

Zanim naprawdę zaczniemy, musisz skonfigurować informację o połączeniu ze swoją
bazą danych. Zgodnie z konwencją informacja ta zapisywana jest w pliku
``app/config/parameters.yml``:

.. code-block:: yaml
   :linenos:

    # app/config/parameters.yml
    parameters:
        database_driver:    pdo_mysql
        database_host:      localhost
        database_name:      test_project
        database_user:      root
        database_password:  password

    # ...

.. note::

    Definiowanie konfiguracji przez ``parameters.yml`` to tylko konwencja.
    Parametry określone w tym pliku są odnoszone do głównego pliku konfiguracyjnego,
    w którym konfigurowana jest Doctrine:

    .. configuration-block::

        .. code-block:: yaml
           :linenos:

            # app/config/config.yml
            doctrine:
                dbal:
                    driver:   "%database_driver%"
                    host:     "%database_host%"
                    dbname:   "%database_name%"
                    user:     "%database_user%"
                    password: "%database_password%"

        .. code-block:: xml
           :linenos:
           
            <!-- app/config/config.xml -->
            <doctrine:config>
                <doctrine:dbal
                    driver="%database_driver%"
                    host="%database_host%"
                    dbname="%database_name%"
                    user="%database_user%"
                    password="%database_password%"
                >
            </doctrine:config>

        .. code-block:: php
           :linenos:
           
            // app/config/config.php
            $configuration->loadFromExtension('doctrine', array(
                'dbal' => array(
                    'driver'   => '%database_driver%',
                    'host'     => '%database_host%',
                    'dbname'   => '%database_name%',
                    'user'     => '%database_user%',
                    'password' => '%database_password%',
                ),
            ));

    Przez oddzielenie informacji z bazy danych do odrębnego pliku można łatwo
    przechowywać różne wersje pliku na każdym serwerze. Można również łatwo
    przechowywać poza projektem konfigurację bazy danych (lub jakieś poufne
    informacje), na przykład wewnątrz konfiguracji Apache. Więcej informacji na
    ten temat można uzyskać w artykule :doc:`/cookbook/configuration/external_parameters`.

Teraz, gdy Doctrine posiada informacje o bazie danych, można użyć tej biblioteki
do utworzenia bazy danych:

.. code-block:: bash

    $ php app/console doctrine:database:create

.. sidebar:: Konfiguracja bazy danych do UTF8

    Częstym błędem, który popełniają nawet doświadczeni programiści jest rozpoczęcie
    projektu Symfony2 bez ustawienia domyślnych wartości ``charset`` i ``collation``
    dla swojej bazy danych, co skutkuje łacińskim porządkiem sortowania, który jest
    domyślny dla większości systemów baz danych. Mogą nawet pamiętać, aby to zrobić
    za pierwszym razem, ale zapominają że czynią to już po uruchomieniu dość popularnych
    poleceń w czasie programowania:

    .. code-block:: bash

        $ php app/console doctrine:database:drop --force
        $ php app/console doctrine:database:create

    Nie ma sposobu aby skonfigurować te wartości domyślne wewnątrz Doctrine.
    Jedyną możliwością rozwiązania tego problemu jest skonfigurowanie tych wartości
    na poziomie serwera.

    Ustawienie domyślne UTF8 dla MySQL jest tak proste, jak dodanie kilku linii
    do pliku konfiguracyjnego serwera (przeważnie ``my.cnf``):

    .. code-block:: ini

        [mysqld]
        collation-server = utf8_general_ci
        character-set-server = utf8

.. index::
      pair: SQLite; stosowanie 

Stosowanie SQLite
~~~~~~~~~~~~~~~~~

Jeśli chcesz stosować bazę danych SQLite, musisz ustawić ścieżkę do pliku bazy
danych SQLite:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        doctrine:
            dbal:
                driver: pdo_sqlite
                path: "%kernel.root_dir%/sqlite.db"
                charset: UTF8

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <doctrine:config
            driver="pdo_sqlite"
            path="%kernel.root_dir%/sqlite.db"
            charset="UTF-8"
        >
            <!-- ... -->
        </doctrine:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'dbal' => array(
                'driver'  => 'pdo_sqlite',
                'path'    => '%kernel.root_dir%/sqlite.db',
                'charset' => 'UTF-8',
            ),
        ));


Utworzenie klasy encji
~~~~~~~~~~~~~~~~~~~~~~

Załóżmy, że budujemy aplikację w której powinny być wyświetlane produkty. Nawet
bez myślenia o Doctrine lub bazach danych wiesz już, że do reprezentowania produktów
potrzebny jest obiekt ``Product``. Utworzymy tą klasę wewnątrz katalogu ``Entity``
w ``AcmeStoreBundle``::

    // src/Acme/StoreBundle/Entity/Product.php
    namespace Acme\StoreBundle\Entity;

    class Product
    {
        protected $name;

        protected $price;

        protected $description;
    }

Klasa ta, często nazywana "encją" (co oznacza podstawową klasę przechowującą dane),
jest prosta i pomaga spełnić wymóg biznesowy prezentowania produktów w aplikacji.
Klasa ta na razie nie może być utrwalona w bazie danych - jest to tylko prosta
klasa PHP.

.. tip::

    Gdy poznasz koncepcje stojące za Doctrine, powinieneś sam tworzyć encje:
    
    .. code-block:: bash

        $ php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Product" --fields="name:string(255) price:float description:text"

.. index::
    single: Doctrine; dodawanie metadanych odwzorowania

.. _book-doctrine-adding-mapping:

Dodawanie informacji odwzorowania
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine umożliwia pracę z bazami danych w sposób o wiele bardziej interesujacy
niż tylko pobieranie wierszy do tablic z tabel kolumnowych. Zamiast tego, Doctrine
umożliwia utrwalanie w bazie danych całych obiektów. Działa to poprzez odwzorowanie
(mapowanie) klasy na tabelę bazy danych a właściwości klasy na kolumny tabeli:

.. image:: /images/book/doctrine_image_1.png
   :align: center

By to wykonać w Doctrine trzeba utworzyć "metadane" lub w konfiguracji ustawić
odwzorowanie klasy Product i jej właściwości na bazę danych. Metadane można określić
w kilku różnych formatach, włączając w to YAML, XML lub bezpośredni w klasie
``Product`` poprzez adnotacje:

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="product")
         */
        class Product
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length=100)
             */
            protected $name;

            /**
             * @ORM\Column(type="decimal", scale=2)
             */
            protected $price;

            /**
             * @ORM\Column(type="text")
             */
            protected $description;
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            table: product
            id:
                id:
                    type: integer
                    generator: { strategy: AUTO }
            fields:
                name:
                    type: string
                    length: 100
                price:
                    type: decimal
                    scale: 2
                description:
                    type: text

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\StoreBundle\Entity\Product" table="product">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO" />
                </id>
                <field name="name" column="name" type="string" length="100" />
                <field name="price" column="price" type="decimal" scale="2" />
                <field name="description" column="description" type="text" />
            </entity>
        </doctrine-mapping>

.. note::

    W pakiecie można zdefiniować metadane tylko w jednorodnym formacie. Na przykład,
    nie jest możliwe zmieszanie definicji w formacie YAML z adnotacjami w pliku
    z definicją klasy encji PHP.

.. tip::

    W konfiguracji nazwa tabeli jest opcjonalna i jeżeli zostanie pominięta, to
    automatycznie zostanie przyjęta nazwa z klasy encji.


Doctrine umożliwia wybór typu pola spośród szerokiej gamy różnych rodzajów pól,
każdy z własnymi opcjami. Więcej informacji na ten temat można znaleźć w rozdziale
:ref:`book-doctrine-field-types`.

.. seealso::

    Można również zapoznać się z `Basic Mapping Documentation`_ w celu poznania
    szczegółowej informacji o odzwzorowaniu. Jeżeli stosuje się adnotacje, to trzeba
    poprzedzić wszystkie adnotacje przedrostkiem ``ORM\`` (np. ``ORM\Column(..)``),
    co nie jest opisane w dokumentacji Doctrine. Musi się również dołączyć wyrażenie
    ``use Doctrine\ORM\Mapping as ORM;``, które importuje przedrostek adnotacji ORM.

.. caution::

    Należy uważać aby nazwa klasy i właściwości nie zostały odwzorowane na chronione
    słowa kluczowe SQL (takie jak ``group`` lub ``user``). Na przykład, jeżeli
    nazwa klasy encji, to ``Group``, to domyślnie nazwa tabeli przybierze nazwę
    ``group``, co powodować będzie błąd SQL w niektórych silnikach.
    Zobacz rozdział `Reserved SQL keywords`_ w dokumentacji Doctrine, w celu
    poznania sposobu prawidłowego sposobu rozwiązania konfliktu tych nazw.
    Ewentualnie, jeżeli ma się wolną rękę w wyborze schematu bazy danych,
    to wystarczy odwzorować inną nazwę tabeli lub kolumny. Zobacz do rozdziałów
    `Persistent classes`_ i `Property Mapping`_ w dokumentacji Doctrine.

.. note::

    W przypadku korzystania z innej biblioteki lub programu (np. Doxygen), które
    wykorzystują adnotacje, trzeba umieścić w klasie z adnotacją wyrażenie
    ``@IgnoreAnnotation``, aby wskazać, które adnotacje mają być ignorowane przez
    Symfony. Na przykład, aby uniknąć zrzucania wyjątku przez adnotację ``@fn``
    trzeba dodać następujące wyrażenie::

        /**
         * @IgnoreAnnotation("fn")
         */
        class Product
        // ...

.. index::
      single: metoda akcesor

Wygenerowanie metod akcesorów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chociaż już Doctrine wie jak utrwalić obiekt ``Product`` w bazie danych, sama klasa
nie jest jeszcze przydatna. Ponieważ ``Product`` jest zwykłą klasą PHP, to potrzeba
utworzyć metody akcesorów pobierających i ustawiających (*ang. getter i setter*)
(tj. ``getName()``, ``setName()``) w celu uzyskania dostępu do właściwości tego
obiektu (gdyż właściwości te są chronione). Doctrine może utworzyć te akcesory
w wyniku polecenia:

.. code-block:: bash

    $ php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product

Zastosowanie tego polecenia daje pewność, że w klasie ``Product`` zostaną wygenerowane
wszystkie niezbędne akcesory. Polecenie to jest bezpieczne – można uruchamiać je
w kółko - wygeneruje ono tylko nie istniejące akcesory (czyli nie nadpisuje istniejących
metod).

.. caution::

    Należy pamiętać, że generator encji Doctrine wytwarza proste akcesory.
    Trzeba sprawdzić wygenerowana encje i dostosować logikę tych akcesorów do
    własnych potrzeb.
    
.. sidebar:: Więcej o ``doctrine:generate:entities``

    Przy pomocy polecenia ``doctrine:generate:entities`` można:

        * generować akcesory;

        * generować klasy repozytorium konfigurowane adnotacją
          ``@ORM\Entity(repositoryClass="...")``;

        * generować właściwy konstrukltor dla relacji 1:n i n:m.

    Polecenie ``doctrine:generate:entities`` zabezpiecza kopię zapasową oryginalego
    pliku ``Product.php`` mianując ją nazwą ``Product.php~``. W niektórych przypadkach
    obecność tego pliku może powodować błąd "Cannot redeclare class". Można wówczas
    ten plik bezpiecznie usunąć.

    Proszę zauważyć, że nie musi się korzystać z powyższego polecenia.
    Doctrine nie jest uzależniona od wygenerowania tego polecenia. Wystarczy
    upewnić się, jak w zwykłej klasie PHP, czy wszystkie chronione właściwości
    klasy mają swoje akcesory. Polecenie to zostało utworzone ponieważ używanie
    Doctrine z linii poleceń jest popularne.

Można wygenerować wszystkie znane encje pakietu (tj. wszystkie klasy PHP określone
w informacji odwzorowania Doctrine) lub w całej przestrzeni nazw:

.. code-block:: bash

    $ php app/console doctrine:generate:entities AcmeStoreBundle
    $ php app/console doctrine:generate:entities Acme

.. note::

    Dla Doctrine jest wszystko jedno, czy właściwości są chronione czy prywatne,
    lub czy istnieją akcesory dla właściwości. Akcesory są generowane tylko dlatego,
    że potrzebna jest interakcja z obiektem PHP.

.. index::
      single: Doctrne; tworzenie schematu
      single: Doctrne; tworzenie tabel bazy danych

Utworzenie schematu i tabel bazy danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Mamy już teraz użyteczną klasę ``Product`` z informacją odwzorowania instruującą
Doctrine jak tą klase obsługiwać. Nie mamy jeszcze odpowiadającej tej klasie
tabeli ``product`` w bazie danych. Doctrine może automatycznie tworzyć tabele
bazy danych potrzebne dla każdej znanej encji w aplikacji. Aby to zrobić,
wystarczy uruchomić polecenie:


.. code-block:: bash

    $ php app/console doctrine:schema:update --force

.. tip::

    Tak naprawdę polecenie to jest bardzo potężne. Porównuje ono informacje o tym
    jak powinna wyglądać baza danych (na podstawie informacji odwzorowania encji)
    z informacją o tym jak wygląda ona obecnie i generuje wyrażenia SQL potrzebne
    do zaktualizowania bazy danych. Innymi słowami, jeżeli doda się nowe właściwości
    w metadanych odwzorowania dla klasy Product i uruchomi się to zadanie ponownie,
    to zostanie wygenerowane wyrażenie "alter table" potrzebne do dodania nowej
    kolumny do istniejącej tabeli ``product``.

    Lepszym sposobem skorzystania z zaawansowanych możliwości tego polecenia jest
    użycie :doc:`migracji</bundles/DoctrineMigrationsBundle/index>`, które umożliwiają
    wygenerowanie tych wyrażeń SQL i zabezpieczenie ich w klasach migracyjnych,
    które można uruchamiać systematycznie na swoim serwerze produkcyjnym w celu
    śledzenia i migracji schematu bazy danych, bezpiecznie i niezawodnie.

Nasza baza danych ma teraz w pełni funkcjonalną tabelę ``product``, która zgodna
jest z określonymi metadanymi.


Utrwalanie obiektów w bazie danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Teraz mamy już encję ``Product`` odwzorowaną w odpowiadającej jej tabeli ``product``,
można więc przekazać dane do bazy danych. Dokonanie tego z poziomu kontrolera jest
całkiem proste. Dodamy następujaca metodę do ``DefaultController`` pakietu:

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php

    // ...
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;

    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice('19.99');
        $product->setDescription('Lorem ipsum dolor');

        $em = $this->getDoctrine()->getManager();
        $em->persist($product);
        $em->flush();

        return new Response('Created product id '.$product->getId());
    }

.. note::

    Jeśli wykonujesz nasz przykład, to aby zobaczyć jak to działa musisz utworzyć
    trasę wskazującą na tą akcję.

Spójrzmy na powyższy kod bardziej szczegółowo:


* **linie 9-12** W tej sekcji tworzymy instancję klasy i działamy z obiektem ``$product``
  jak z innym zwykłym obiektem PHP;

* **linia 14** W tej linii pobieramy obiekt *menadżera encji* Doctrine, który jest
  odpowiedzialny za obsługę procesu utrwalania i pobierania obiektów z formularza
  do bazy danych;

* **linia 15** Metoda ``persist()`` powiadamia Doctrine aby "zarządzała" obiektem
  ``$product``. W rzeczywistości to nie powoduje wprowadzenia zapytania do bazy danych
  (na razie);

* **linia 16** Gdy wywoływana jest metoda ``flush()``, Doctrine przeszukuje wszystkie
  zarządzane obiekty, by sprawdzić, czy muszą one zostać utrwalone w bazie danych.
  W naszym przykładzie obiekt ``$product`` nie został jeszcze utrwalony, tak więc
  menadżer encji wykona zapytanie ``INSERT`` i utworzony zostanie wiersz w tabeli
  ``product``.

.. note::

  W rzeczywistości, ponieważ Doctrine ma informacje o wszystkich zarządzanych encjach,
  to gdy wywoła się metodę ``flush()``, przeliczy ona całkowity wskaźnik zmian
  i wykona możliwie najlepsze zapytanie (zapytania). Przykładowo, jeżeli do utrwalenia
  jest w sumie 100 obiektów ``Product`` i wywoła się metodę ``flush()``, to Doctrine
  utworzy pojedyncze wyrażenie i ponownie go użyje dla każdego zapisu. Ten wzorzec
  projektowy jest nazywany *wzorcem jednostki pracy* (*ang. Unit of Work Pattern*) [1]_
  a jest używany, bo jest szybki i skuteczny.

Podczas tworzenia lub aktualizowania obiektów działanie jest zawsze takie samo.
W następnym rozdziale poznasz, jak Doctrine jest wystarczająco inteligentny aby
automatycznie wystawiać zapytanie ``UPDATE``, jeżeli rekord już istnieje w bazie danych.

.. tip::

    Doctrine dostarcza bibliotekę pozwalającą programowo załadować dane testowe
    do projektu. Więcej informacji uzyskasz w :doc:`/bundles/DoctrineFixturesBundle/index`.

Pobieranie obiektów z bazy danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pobieranie z powrotem obiektów z bazy danych jest jeszcze bardziej łatwiejsze.
Na przykład, załóżmy, że skonfigurowana została trasa do wyświetlania konkretnego
produktu na podstawie jego wartości ``id``::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'No product found for id '.$id
            );
        }

        // ... zrobić coś, na przykład przekazać obiekt $product do szablonu
    }

.. tip::

    Możesz osiągnąć odpowiednik tego bez pisania jakiegokolwiek kodu używając skrótu
    ``@ParamConverter``. Zobacz dokumentację
    :doc:`FrameworkExtraBundle</bundles/SensioFrameworkExtraBundle/annotations/converters>`.
    
Gdy przesyła się zapytanie dotyczące określonego typu obiektu, zawsze używa się czegoś,
co nazywa się "repozytorium". Możesz myśleć o repozytorium jak o klasie PHP, której
jedynym zadaniem jest pomoc w pobieraniu encji pewnych klas. Można uzyskać dostęp do
obiektu repozytorium dla klasy encji poprzez::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    Łańcuch ``AcmeStoreBundle:Product`` jest skrótem, jaki można używać zawsze
    w Doctrine zamiast pełnej nazwy encji (tj. ``Acme\StoreBundle\Entity\Product``).
    Będzie to działać dopóty ważna jest encja w przestrzeni nazw ``Entity`` pakietu.

Po utworzeniu repozytorium ma się dostęp do wszelkiego rodzaju przydatnych metod::

    // zapytanie przez klucz główny (zwykle "id")
    $product = $repository->find($id);

    // dynamiczne nazwy kolumn odnajdywane na podstawie wartości kolumnowej
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // odnajdywanie *all* produktów
    $products = $repository->findAll();

    // odnajdywanie grupy produktów na podstawie dowolnej wartości kolumnowej
    $products = $repository->findByPrice(19.99);

.. note::

    Oczywiście można również zadawać bardziej złożone zapytania o których można
    dowiedzieć się więcej w rozdziale :ref:`book-doctrine-queries`.

Można również wykorzystać przydatne metody ``findBy`` i ``findOneBy`` do łatwego
pobierania obiektu na podstawie różnych warunków::

    // zapytanie o jeden produkt o określonej nazwie i cenie
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // zapytanie o wszystkie produkty pasujace do określonej nazwy, posortowane wg. ceny
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price' => 'ASC')
    );

.. tip::

    Można zobaczyć, jak wiele zapytań jest wykonywanych podczas generowania strony
    na dolnym pasku debugowania, w prawym dolnym rogu.

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 50
       :width: 350

    Po kliknięciu na ikonę otworzy się profiler, pokazując dokładnie wykonane zapytania.

Aktualizacja obiektu
~~~~~~~~~~~~~~~~~~~~

Po pobraniu obiektu z Doctrine, jego aktualizacja jest prosta. Załóżmy, że mamy
trasę, która mapuje ``id`` produktu do kontrolera w celu przeprowadzenia aktualizacji
danych::

    public function updateAction($id)
    {
        $em = $this->getDoctrine()->getManager();
        $product = $em->getRepository('AcmeStoreBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'No product found for id '.$id
            );
        }

        $product->setName('New product name!');
        $em->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

Aktualizacja obiektu obejmuje tylko trzy kroki:

#. pobranie obiektu przez Doctrine;
#. zmodyfikowanie obiektu;
#. wywołanie metody ``flush()`` w menadżerze encji

Proszę zauważyć, że wywołanie ``$em->persist($product)`` nie jest konieczne.
Przypominamy, że metoda ta jedynie informuje Doctrine, aby zarządzało lub
"przyglądało się" obiektowi ``$product``. W naszym przypadku, ponieważ obiekt
``$product`` został już pobrany przez Doctrine, więc jest już on zarządzany.

Usunięcie obiektu
~~~~~~~~~~~~~~~~~

Usuwanie obiektu jet bardzo podobne, ale wymaga wywołania metody ``remove()``
menadżera encji::

    $em->remove($product);
    $em->flush();

Jak można się spodziewać, metoda ``remove()`` powiadamia Doctrine, że chce się
usunąć daną encję z bazy danych. Zapytanie ``DELETE`` nie jest wykonywane, do
czasu wywołania metody ``flush()``.

.. _`book-doctrine-queries`:

Zapytania o obiekty
-------------------

Widziałeś już, jak obiekt repozytorium umożliwia uruchomienie podstawowych zapytań
bez specjalnego wysiłku::

    $repository->find($id);

    $repository->findOneByName('Foo');

Oczywiście Doctrine umożliwia również pisanie bardziej złożonych zapytań przy
użyciu Doctrine Query Language (DQL). DQL jest podobny do SQL, z tą różnicą, że
trzeba sobie wyobrazić, że tu odpytywane są obiekty klasy encji (np. ``Product``)
a nie wiersze tabeli (np. ``product``).

Podczas odpytywania w Doctrine, ma się dwie możliwości: pisanie czystych zapytań
Doctrine lub stosowanie konstruktora zapytań Doctrine.

Zapytania o obiekty z DQL
~~~~~~~~~~~~~~~~~~~~~~~~~

Wyobraź sobie, że chcesz zapytać o produkty, ale tylko takie, które kosztują więcej
niż ``19.99`` i są uporządkowane od najtańszych do najdroższych. Wewnątrz kontrolera
utwórz następujący kod::

    $em = $this->getDoctrine()->getManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');

    $products = $query->getResult();

Jeżeli jesteś zaznajomiony z SQL, to z DQL powinieneś się czuć bardzo naturalnie.
Największą różnicą jest to, że w DQL powinieneś myśleć w kategoriach "obiektów"
zamiast wierszy bazy danych. Z tego powodu należy wybrać ``AcmeStoreBundle:Product``
i następnie oznaczyć jego alias jako ``p``.

Metoda ``getResult()`` zwraca tablicę wyników. Jeżeli odpytuje się tylko jeden
obiekt, to zamiast niej można użyć metody ``getSingleResult()``::

    $product = $query->getSingleResult();

.. caution::

    Metoda ``getSingleResult()`` zrzuca wyjątek ``Doctrine\ORM\NoResultExceptionexception``
    jeśli zwracany jest brak wyników zapytania oraz wyjątek ``Doctrine\ORM\NonUniqueResultException``
    jeśli zwracanych jest więcej niż jeden wynik. Jeżeli używa się tą metodę, to
    zachodzi potrzeba opakowania kodu w blok ``try-catch`` i zapewnienie aby zwracany
    był tylko jeden wynik (jeśli wyszukuje się coś, co może zwrócić więcej niż jeden
    wynik)::

        $query = $em->createQuery('SELECT ...')
            ->setMaxResults(1);

        try {
            $product = $query->getSingleResult();
        } catch (\Doctrine\Orm\NoResultException $e) {
            $product = null;
        }
        // ...

Składnia DQL jest bardzo mocna, umożliwiająca łatwe tworzenie złączeń pomiędzy encjami
(ten temat jest omówiony w sekcji :ref:`Relations<book-doctrine-relations>`),
grupowanie itd. Więcej informacji można uzyskać w rozdziale `Doctrine Query Language`_
oficjalnej dokumentacji Doctrine.

.. sidebar:: Konfiguracja parametrów

    Należy zwrócić uwagę na metodę ``setParameter()``. Podczas pracy z Doctrine,
    dobrym pomysłem jest ustawienie wszystkich wartości zewnętrznych jako
    "wieloznaczników", tak jak w pierwszym przykładzie:
    

    .. code-block:: text

        ... WHERE p.price > :price ...

    Następnie można ustawić wartość wieloznacznika price przez wywołanie metody
    ``setParameter()``::

        ->setParameter('price', '19.99')

    Stosowanie parametrów zamiast bezpośredniego wprowadzania wartości w łańcuchu
    zapytania zapobiega atakom wstrzyknięcia SQL i powinno być zawsze stosowane.
    Jeśli używa się wielu parametrów, to można ustawić ich wartości naraz stosując
    metodę ``setParameters()``::

        ->setParameters(array(
            'price' => '19.99',
            'name'  => 'Foo',
        ))

.. index::
      single: Doctrine; QueryBuilder 

Stosowanie konstruktora zapytań Doctrine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Zamiast pisać bezpośrednio zapytania, można alternatywnie wykorzystać obiekt
``QueryBuilder`` Doctrine, udostęþniający obiektowo-zorientowany interfejs.
Jeżeli używa się z IDE, to można również skorzystać z autouzupełniania podczas
wpisywania nazw metod. Z poziomu kontrolera::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();

    $products = $query->getResult();

Obiekt ``QueryBuilder`` zawiera wszystkie niezbędne metody do do budowy zapytania.
Przez wywołanie metody ``thegetQuery()`` konstruktor zapytań zwraca zwykły obiekt
``Query``, który jest taki sam, jak obiekt zbudowany w poprzednim rozdziale.

Więcej informacji o konstruktorze zapytań Doctrine można znaleźć w dokumentacji
`Query Builder`_.

Własne klasy repozytorium
~~~~~~~~~~~~~~~~~~~~~~~~~

W poprzednich rozdziałach rozpoczęliśmy konstruowanie i używanie bardziej złożonych
zapytań wewnątrz kontrolera. W celu izolacji, testowania i ponownego wykorzystania
zapytań, dobrym pomysłem jest utworzenie własnej klasy repozytorium dla encji
i dodanie tam metod tworzących logikę zapytania.

Aby to zrobić, należy dodać nazwę klasy repozytorium do definicji odwzorowania.

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity(repositoryClass="Acme\StoreBundle\Entity\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            repositoryClass: Acme\StoreBundle\Entity\ProductRepository
            # ...

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->

        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product"
                    repository-class="Acme\StoreBundle\Entity\ProductRepository">
                    <!-- ... -->
            </entity>
        </doctrine-mapping>

Doctrine może samo wygenerować klasę repozytorium po uruchomieniu tego samego
polecenia, które użyliśmy wcześniej do wygenerowania metod akcesorów:

.. code-block:: bash

    $ php app/console doctrine:generate:entities Acme

Następnie dodajemy nowa metodę ``findAllOrderedByName()`` do nowo utworzonej klasy
repozytorium. Metoda ta będzie przepytywać wszystkie encje ``Product`` w kolejności
alfabetycznej.

.. code-block:: php
   :linenos:

    // src/Acme/StoreBundle/Entity/ProductRepository.php
    namespace Acme\StoreBundle\Entity;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function findAllOrderedByName()
        {
            return $this->getEntityManager()
                ->createQuery('SELECT p FROM AcmeStoreBundle:Product p ORDER BY p.name ASC')
                ->getResult();
        }
    }

.. tip::

    Menadżer encji może być dostępny poprzez ``$this->getEntityManager()``
    z poziomu repozytorium.

Możesz używać tej nowej metody, podobnie jak domyślnych metod wyszukujących repozytorium::

    $em = $this->getDoctrine()->getManager();
    $products = $em->getRepository('AcmeStoreBundle:Product')
                ->findAllOrderedByName();

.. note::

    Podczas stosowania własnej klasy repozytorium nadal ma się dostęp do domyślnych
    metod, takich jak ``find()`` i ``findAll()``.

.. _`book-doctrine-relations`:

Relacje (powiązania) encji
--------------------------

Załóżmy, że produkty w naszej aplikacji należą do jednej "kategorii".
W tym przypadku będziemy potrzebować obiektu ``Category`` i jakiegoś sposobu
odzwierciedlenia relacji obiektu ``Product`` do obiektu ``Category``.
Rozpocznijmy od utworzenia encji ``Category``. Ponieważ wiesz już, że ostatecznie
trzeba będzi utrzymać klasę poprzez Doctrine, to możemy pozwolić, aby Doctrine
utworzyła tą klasę.

.. code-block:: bash

    $ php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Category" --fields="name:string(255)"

Zadanie to wygeneruje encję ``Category``, z polami ``id`` i ``name``,
oraz związanymi funkcjami akcesorów.

Metadane odwzorowania relacji
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aby powiązać encje ``Category`` i ``Product`` trzeba rozpocząć od utworzenia
właściwości ``products`` w klasie ``Category``:

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/StoreBundle/Entity/Category.php

        // ...
        use Doctrine\Common\Collections\ArrayCollection;

        class Category
        {
            // ...

            /**
             * @ORM\OneToMany(targetEntity="Product", mappedBy="category")
             */
            protected $products;

            public function __construct()
            {
                $this->products = new ArrayCollection();
            }
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/StoreBundle/Resources/config/doctrine/Category.orm.yml
        Acme\StoreBundle\Entity\Category:
            type: entity
            # ...
            oneToMany:
                products:
                    targetEntity: Product
                    mappedBy: category
            # don't forget to init the collection in entity __construct() method

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Category.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\StoreBundle\Entity\Category">
                <!-- ... -->
                <one-to-many field="products"
                    target-entity="product"
                    mapped-by="category"
                />

                <!-- don't forget to init the collection in entity __construct() method -->
            </entity>
        </doctrine-mapping>

Po pierwsze, ponieważ obiekt ``Category`` będzie odnosić się do wielu obiektów
klasy ``Product``, to dodawana jest właściwość będąca tablicą produktów w celu
przechowywania tych obiektów ``Product``. Dla przypomnienia, nie jest tak dlatego,
że Doctrine wymaga tego rozwiązania, ale dlatego, że sensowne jest przechowywanie
tablicy obiektów ``Product`` dla każdej kategorii.

.. note::

    Kod w metodzie ``__construct()`` jest ważny, ponieważ Doctrine wymaga właściwości
    ``$products`` będącej obiektem ``ArrayCollection``. Obiekt ten wygląda i działa
    prawie tak samo jak tablica, ale ma dodatkową elastyczność. Jeżeli jest to dla
    Ciebie niewygodne, nie przejmuj się. Wystarczy sobie wyobrazić, że jest to tablica.

.. tip::

   Wartość ``targetEntity`` w adnotacji powyżej prezentowanej może odwoływać się
   do jakiejkolwiek encji z ważną przestrzenią nazw, nie tylko encji określonych
   w tej samej klasie. Aby odnieść ``targetEntity`` do encji zdefiniowanych w innej
   klasie lub pakiecie, trzeba wprowadzić pełną nazwę przestrzeni nazw jako wartość
   ``targetEntity``.

Następnie, ponieważ każda klasa ``Product`` odnosi się dokładnie do jednego obiektu
``Category``, dodamy właściwość ``$category`` do klasy ``Product``:

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/StoreBundle/Entity/Product.php

        // ...
        class Product
        {
            // ...

            /**
             * @ORM\ManyToOne(targetEntity="Category", inversedBy="products")
             * @ORM\JoinColumn(name="category_id", referencedColumnName="id")
             */
            protected $category;
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            manyToOne:
                category:
                    targetEntity: Category
                    inversedBy: products
                    joinColumn:
                        name: category_id
                        referencedColumnName: id

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\StoreBundle\Entity\Product">
                <!-- ... -->
                <many-to-one field="category"
                    target-entity="products"
                    join-column="category"
                >
                    <join-column
                        name="category_id"
                        referenced-column-name="id"
                    />
                </many-to-one>
            </entity>
        </doctrine-mapping>

Na koniec, teraz dodamy nową właściwość do obu klas ``Category`` i ``Product``,
powiadamiająca Doctrine, aby wygenerowało brakujące metody akcesorów:

.. code-block:: bash

    $ php app/console doctrine:generate:entities Acme

Zignorujmy na moment metadane Doctrine. Teraz mamy dwie klasy, ``Category``
i ``Product`` z naturalną relacją jeden-do-wielu. Klasa ``Category`` przechowuje
tablicę obiektów klasy ``Product`` zawierajaca produkty jednej kategorii. Innymi
słowami, mamy skonstruowane potrzebne klasy. Fakt, że muszą one zostać utrwalone
w bazie danych, jest kwestią wtórną

Teraz spójrz na metadane sformułowane powyżej właściwości ``$category`` w klasie
``Product``. Informacja ta powiadamia Doctrine, że powiązana klasa jest kategorią
i że powinna przechowywać identyfikator ``id`` rekordu w polu ``category_id``,
które istnieje w tabeli ``product``. Innymi słowami, powiązany obiekt ``Category``
będzie przechowywane właściwości ``$category``, ale w tle, Doctrine będzie utrzymywać
tą relację przez przechowywanie wartości ``id`` kategorii w kolumnie ``category_id``
tabeli ``product``.

.. image:: /images/book/doctrine_image_2.png
   :align: center

Metadana powyżej właściwości ``$products`` obiektu ``Category`` jest mniej ważna
i tylko powiadamia Doctrine aby wyszukał właściwość ``Product.category`` w celu
ustalenia jaka relacja została odwzorowana.

Przed kontynuowaniem, należy się upewnić, że Doctrine jest poinformowane o nowej
tablicy ``category`` i kolumnie ``product.category_id`` oraz nowym kluczu zewnętrznym:

.. code-block:: bash

    $ php app/console doctrine:schema:update --force

.. note::

    Zadanie to powinno być wykonywane tylko w czasie programowania. W celu
    poznania bardziej solidnej metody systematycznego aktualizowania produkcyjnej
    bazy danych, przeczytaj artykuł
    :doc:`Doctrine migrations</bundles/DoctrineMigrationsBundle/index>`.

Saving Related Entities
~~~~~~~~~~~~~~~~~~~~~~~

Teraz możemy zobaczyć jak działa nowy kod. Wyobraź sobie, że jesteś w kontrolerze::

    // ...

    use Acme\StoreBundle\Entity\Category;
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;

    class DefaultController extends Controller
    {
        public function createProductAction()
        {
            $category = new Category();
            $category->setName('Main Products');

            $product = new Product();
            $product->setName('Foo');
            $product->setPrice(19.99);
            // relate this product to the category
            $product->setCategory($category);

            $em = $this->getDoctrine()->getManager();
            $em->persist($category);
            $em->persist($product);
            $em->flush();

            return new Response(
                'Created product id: '.$product->getId().' and category id: '.$category->getId()
            );
        }
    }

Teraz pojedynczy wiersz jest dodawany do obu tabel ``category`` i ``product``.
Kolumna ``product.category_id`` dla nowego produktu jest ustawiana na ``id``
nowej kategorii. Doctrine sam zarządza utrzymaniem tej relacji.

Pobieranie powiązanych obiektów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Gdy zachodzi potrzeba pobrania powiązanych obiektów, działanie wygląda tak jak
miało to miejsce poprzednio. Najpierw trzeba pobrać obiekt ``$product``
a następnie uzyskać dostęp do powiązanego obiektu ``Category``::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $categoryName = $product->getCategory()->getName();

        // ...
    }

In this example, you first query for a ``Product`` object based on the product's
``id``. This issues a query for *just* the product data and hydrates the
``$product`` object with that data. Later, when you call ``$product->getCategory()->getName()``,
Doctrine silently makes a second query to find the ``Category`` that's related
to this ``Product``. It prepares the ``$category`` object and returns it to
you.
W tym przykładzie, najpierw zapytamy o obiekt ``Product`` w oparciu o ``id`` produktu.
W tym celu sformujemy zapytanie tylko dla danych produktu i hydratów obiektu ``$product``
z tymi danymi. Później, gdy wywołamy ``$product->getCategory()->getName()``,
Doctrine niejawnie wykona drugie zapytanie aby odnaleźć kategorię powiązaną z produktem.
To przygotuje i zwróci obiekt ``$category``.

.. image:: /images/book/doctrine_image_3.png
   :align: center

Ważne jest to, że ma się łatwy dostęp do powiązanej z produktem kategorii, ale
dane kategorii nie są faktycznie pobierane, dopóki się nie zapyta o tą kategorię
(jest to tzw. „wzorzec leniwego ładowania", *ang. Lazily Loaded Pattern*).

Można również zapytać w drugą stronę::

    public function showProductAction($id)
    {
        $category = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Category')
            ->find($id);

        $products = $category->getProducts();

        // ...
    }

W tym przypadku, postępowanie jest takie samo: najpierw pytamy o pojedynczy obiekt
``Category`` a następnie Doctrine wykonuje drugie zapytanie, aby pobrać powiązany
obiekt ``Product``, ale tylko raz, jeśli jest on potrzebny (tj. gdy wywołamy
``->getProducts()``). Zmienna ``$products`` jest tablicą obiektów ``Product``,
które są powiązane z określonym obiektem ``Category`` poprzez ich wartość ``category_id``.

.. sidebar:: Relacje a klasy Proxy

    To "leniwe ładowanie" jest możliwe, ponieważ w razie potrzeby Doctrine zwraca
    obiekt "proxy" w miejsce prawdziwego obiektu. Przeanalizujmy ponownie powyższy
    przykład::

        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $category = $product->getCategory();

        // prints "Proxies\AcmeStoreBundleEntityCategoryProxy"
        echo get_class($category);

    Ten obiekt proxy rozszerza prawdziwy obiekt ``Category``, wyglądając i funkcjonując
    jak on. Różnica jest taka, że przez użycie obiektu proxy, Doctrine może opóźnić
    utworzenie zapytania dla rzeczywistych danych ``Category`` do momentu, w którym
    te dane staną się potrzebne (tj. aż nie wywoła się ``$category->getName()``).

    Klasy proxy są generowane przez Doctrine i przechowywane w katalogu pamięci
    podręcznej. Choć przypuszczalnie nigdy nie będziesz ich zauważał, to ważne jest,
    aby pamiętać, że obiekt ``$category`` jest w rzeczywistości obiektem proxy.

    W następnym rozdziale, podczas pobierania naraz danych produktów i kategorii
    (poprzez *join*), Doctrine zwróci prawdziwy obiekt ``Category``, ponieważ nic
    nie musi być ładowane leniwie.

Łączenie powiązanych rekordów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W powyższych przykładach zostały wykonane dwa zapytania – jedno dla oryginalnego
obiektu (tj. ``Category``) a drugie dla obiektów powiązanych (tj. obiektów ``Product``).

.. tip::

    Pamiętaj, że możesz zobaczyć wszystkie wykonane podczas zapytania zapytania
    na pasku debugowania.

Jeśli wiesz z góry, że będziesz potrzebował dostępu do obu obiektów, to możesz
uniknąć drugiego zapytania przez zastosowanie złączenia w oryginalnym zapytaniu.
Dodamy następującą metodę do klasy ``ProductRepository``::

    // src/Acme/StoreBundle/Entity/ProductRepository.php
    public function findOneByIdJoinedToCategory($id)
    {
        $query = $this->getEntityManager()
            ->createQuery('
                SELECT p, c FROM AcmeStoreBundle:Product p
                JOIN p.category c
                WHERE p.id = :id'
            )->setParameter('id', $id);

        try {
            return $query->getSingleResult();
        } catch (\Doctrine\ORM\NoResultException $e) {
            return null;
        }
    }


Teraz możemy korzystać z tej metody w kontrolerze, aby pytać o obiekt ``Product``
i powiązany z nim obiekt ``Category``::


    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();

        // ...
    }


Więcej informacji o powiązaniach
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Rozdział ten jest wprowadzeniem do popularnego typu relacji encji, *jeden do wielu*.
Więcej zaawansowanych szczegółów i przykładów tego, jak używać inne typy relacji
(czyli  *jeden do jeden*, *wiele do wielu*) znajdziesz w części dokumentacji
Doctrine `Association Mapping`_.

.. note::

    Jeżeli używa się adnotacji, to trzeba poprzedzać wszystkie adnotacje przedrostkiem
    ``ORM\`` (np. ``ORM\OneToMany``), co nie zostało uwzględnione w dokumentacji
    Doctrine. Należy również dołączyć wyrażenie use ``Doctrine\ORM\Mapping as ORM;``,
    które importuje przedrostki adnotacji ORM.

Konfiguracja
------------

Doctrine jest wysoce konfigurowalna, ale prawdopodobnie nigdy nie będziesz musiał
martwić się o większość opcji konfiguracyjnych tej biblioteki. Aby dowiedzieć się
więcej o konfiguracji Doctrine, przeczytaj rozdział
:doc:`reference manual</reference/configuration/doctrine>` w dokumentacji Doctrine.

Wywołania zwrotne cyklu życia encji
-----------------------------------

Czasem zachodzi potrzeba wykonania akcji zaraz przed lub po dodaniu,
zaktualizowaniu lub usunięciu encji. Tego typu akcje są nazywane **wywołaniami
zwrotnymi "cyklu życia" encji**, jako że są one metodami wywołań zwrotnych, które
trzeba wykonać na różnych etapach istnienia encji (tj. gdy encja jest dodawana,
aktualizowana, usuwana itd.).

Jeżeli używa się adnotacji dla określenia metadanych, należy rozpocząć od udostępnienia
wywołań zwrotnych cyklu życia. Nie jest to konieczne, jeśli stosuje się YAML lub XML
do odwzorowywania:

.. code-block:: php-annotations
   :linenos:

    /**
     * @ORM\Entity()
     * @ORM\HasLifecycleCallbacks()
     */
    class Product
    {
        // ...
    }

Teraz możemy powiadomić Doctrine aby wykonała metodę na każdym dostępnym zdarzeniu
w cyklu funkcjonowania encji. Przykładowo załóżmy że, chcemy ustawić utworzoną
kolumnę datową na bieżącą datę, ale tylko wtedy, gdy encja jest pierwszy raz utrwalana
(tj. dołożona):

.. configuration-block::
   :linenos:

    .. code-block:: php-annotations
       :linenos:

        /**
         * @ORM\PrePersist
         */
        public function setCreatedValue()
        {
            $this->created = new \DateTime();
        }

    .. code-block:: yaml
       :linenos:

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            lifecycleCallbacks:
                prePersist: [ setCreatedValue ]

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->

        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product">
                    <!-- ... -->
                    <lifecycle-callbacks>
                        <lifecycle-callback type="prePersist" method="setCreatedValue" />
                    </lifecycle-callbacks>
            </entity>
        </doctrine-mapping>

.. note::

    Powyższy przykład zakłada, że wcześniej utworzyliśmy i odwzorowali właściwość
    ``creates`` (czego tu nie pokazano).


Teraz, tuż przed pierwszym utrwaleniem encji, Doctrine automatycznie wywoła tą
metodę i ustawi pole ``created`` na bieżącą datę.


Może to być powtórzone dla każdego zdarzenia cyklu życia encji, którymi są:

* ``preRemove``
* ``postRemove``
* ``prePersist``
* ``postPersist``
* ``preUpdate``
* ``postUpdate``
* ``postLoad``
* ``loadClassMetadata``

Więcej ogólnych informacji na temat zdarzeń cyklu życia encji i wywołań zwrotnych
tego cyklu można znaleźć w rozdziale `Lifecycle Events`_ dokumentacji Doctrine.

.. sidebar:: Wywołania zwrotne cyklu życia i nasłuchiwanie zdarzeń

    Proszę zauważyć, że metoda ``setCreatedValue()`` nie przejmuje żadnych
    argumentów. Tak jest zawsze w przypadku wywołań zwrotnych cyklu życia encji
    i jest to zamierzone – wywołania zwrotne cyklu życia encji powinny być prostymi
    metodami, które dotyczą wewnętrznego przekształcania danych encji
    (np. ustawienie tworzenia lub aktualizowania pola, generowanie wartości slug).
    
    Jeśli zachodzi potrzeba wykonania bardziej zaawansowanego kodu - takiego jak
    obsługa logowania, czy wysyłania wiadomości e-mail, powinno się zarejestrować
    zewnętrzne klasy do nasłuchiwania lub subskrybcji zdarzeń i dać im dostęp do
    wszystkich potrzebnych zasobów. W celu uzyskania więcej informacji można
    sięgnąć do artykułu :doc:`How to Register Event Listeners and Subscribers
    </cookbook/doctrine/event_listeners_subscribers>`.

Rozszerzenia Doctrine: Timestampable, Sluggable itd.
----------------------------------------------------

Doctrine jest dość elastyczną biblioteką i dostępna jest duża liczba rozszerzeń
osób trzecich, pozwalających łatwo wykonywać na encjach powtarzające się, popularne
zadania. Są to takie rozszerzenia, jak *Sluggable*, *Timestampable*, *Loggable*,
*Translatable* i *Tree*.

Więcej informacji o tym jak znaleźć i stosować te rozszerzenia znajdziesz w artykule
:doc:`How using common Doctrine extensions</cookbook/doctrine/common_extensions>`. 


.. _book-doctrine-field-types:

Doctrine Field Types Reference
------------------------------

Doctrine dostarczana jest z dużą liczbą dostępnych typów pól.
Każdy z nich odwzorowuje typ danych PHP na określony typ kolumny w bazie danych.
Doctrine obsługuje następujące typy danych:

* **Łańcuchy**

  * ``string`` (stosowane dla krótkich łańcuchów)
  * ``text`` (stosowane dla dłuższych łańcuchów)

* **Liczby**

  * ``integer``
  * ``smallint``
  * ``bigint``
  * ``decimal``
  * ``float``

* **Daty i czas** (używaj dla tych pól w PHP obiektu `DateTime`_)

  * ``date``
  * ``time``
  * ``datetime``

* **Inne typy**

  * ``boolean``
  * ``object`` (serializowane i przechowywane w polu ``CLOB``)
  * ``array`` (serializowane i przechowywane w polu ``CLOB``)

Aby uzyskać więcej informacji przeczytaj artykuł `Mapping Types`_ w dokumentacji
Doctrine.

Opcje pól
~~~~~~~~~

Każde pole może mieć przypisany mu zestaw opcji. Dostępne opcje to ``type``
(domyślnie ``string``), ``name``, ``lenght``, ``unique`` i ``nullable``.
Rozpatrzmy kilka przykładów:

.. configuration-block::

    .. code-block:: php-annotations
       :linenos:

        /**
         * A string field with length 255 that cannot be null
         * (reflecting the default values for the "type", "length"
         * and *nullable* options)
         *
         * @ORM\Column()
         */
        protected $name;

        /**
         * A string field of length 150 that persists to an "email_address" column
         * and has a unique index.
         *
         * @ORM\Column(name="email_address", unique=true, length=150)
         */
        protected $email;

    .. code-block:: yaml
       :linenos:

        fields:
            # A string field length 255 that cannot be null
            # (reflecting the default values for the "length" and *nullable* options)
            # type attribute is necessary in yaml definitions
            name:
                type: string

            # A string field of length 150 that persists to an "email_address" column
            # and has a unique index.
            email:
                type: string
                column: email_address
                length: 150
                unique: true

    .. code-block:: xml
       :linenos:

        <!--
            A string field length 255 that cannot be null
            (reflecting the default values for the "length" and *nullable* options)
            type attribute is necessary in yaml definitions
        -->
        <field name="name" type="string" />
        <field name="email"
            type="string"
            column="email_address"
            length="150"
            unique="true"
        />

.. note::

    There are a few more options not listed here. For more details, see
    Doctrine's 
    Istnieje kilka innych opcji, tutaj nie wymienionych. Więcej szczegółów
    znajdziesz w artykule `Property Mapping`_ dokumentacji Doctrine.

.. index::
   single: Doctrine; polecenie ORM z konsoli
   pair: CLI; Doctrine ORM

Polecenia konsoli
-----------------

ORM Doctrine2 oferuje w przestrzeni nazw ``doctrine`` kilka poleceń
konsoli . W celu wyświetlenia tych poleceń uruchom konsolę bez jakichkolwiek
argumentów:

.. code-block:: bash

    $ php app/console

Zostanie wydrukowana lista dostępnych poleceń, z których wiele rozpoczyna się
przedrostkiem ``doctrine:``. Możesz znaleźć więcej informacji o tych poleceniach
(lub dowolnego polecenia Symfony) przez uruchomienie polecenia ``help``.
Na przykład, aby uzyskać informacje o ``doctrine:database:createtask``, uruchom:

.. code-block:: bash

    $ php app/console help doctrine:database:create

Niektóre ważniejsze lub iteresujące zadania, to:

* ``doctrine:ensure-production-settings`` - sprawdza, czy bieżące środowisko jest
  skutecznie skonfigurowane jako produkcyjne. Zawsze powinno być uruchamiane w
  środowisku ``prod``::

  .. code-block:: bash

      $ php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - pozwala Doctrine na introspekcję istniejącej
  bazy danych i utworzenie informacji odwzorowania. Więcej informacji znajdziesz
  w artykule :doc:`/cookbook/doctrine/reverse_engineering`.

* ``doctrine:mapping:info`` - informuje o wszystkich encjach zarejestrowanych
  w Doctrine i o ewentualnych błędach w ich odzwzorowaniu.

* ``doctrine:query:dql`` i ``doctrine:query:sql`` - umożliwia wykonanie zapytań
  DQL lub SQL z linii poleceń.

.. note::

   Aby móc załadować do bazy danych dane testowe, potrzeba zainstalować pakiet
   ``DoctrineFixturesBundle``. Opis jak to zrobić zawarty jest w dokumentacji
   ":doc:`/bundles/DoctrineFixturesBundle/index`".

.. tip::

    Strona ta pokazuje pracę z Doctrine w kontrolerze. Możesz również pracować
    z Doctrine w innym miejscu aplikacji. Metoda
    :method:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller::getDoctrine`
    kontrolera zwraca usługę ``doctrine``, z którą możesz pracować w ten sam sposób
    w każdym miejscu aplikacji, przez wstrzyknięcie tej usługi do własnych usług.
    Przeczytaj :doc:`/book/service_container` w celu uzyskania więcej informacji
    o tworzeniu własnych usług.

Podsumowanie
------------

Stosując Doctrine można skupić się na obiektach i na tym jak są one potrzebne
w aplikacji, nie martwiąc się o ich utrwalenie a bazie danych. Dzieje się tak,
bo Doctrine umożliwia używanie obiektów PHP do przechowywania danych i odwzorowuje
je do określonych tabel baz danych, wykorzystując informacje metadanych odwzorowania.

Pomimo, ze Doctrine działa wg. prostej koncepcji, to jest bardzo silną biblioteką,
umożliwiająca tworzenie złożonych zapytań i wykorzystywać zdarzenia, pozwalając
na wykonywanie różnych akcji na wszystkich etapach życia encji.

Więcej informacji o Doctrine znajdziesz w :doc:`cookbook</cookbook/index>`,
gdzie znajdują sie następujące artykuły:

* :doc:`/bundles/DoctrineFixturesBundle/index`
* :doc:`/cookbook/doctrine/common_extensions`

.. rubric:: Przypisy

.. [1] Wzorzec ten opisany został po raz pierwszy w *Pattern of Enterprise Application
       Architecture* przez Martina Fowlera. Polskojęzyczny opis wzorca znajduje się
       w książce "PHP Obiekty, wzorce, narzędzia" Matta Zandstra, wyd. III Helion S.A.
       2011.


.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Basic Mapping Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html
.. _`Query Builder`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/query-builder.html
.. _`Doctrine Query Language`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/dql-doctrine-query-language.html
.. _`Association Mapping`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Mapping Types`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#doctrine-mapping-types
.. _`Property Mapping`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Events`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/events.html#lifecycle-events
.. _`Reserved SQL keywords`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#quoting-reserved-words
.. _`Persistent classes`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/basic-mapping.html#persistent-classes
