.. highlight:: php
   :linenothreshold: 2

.. index::
   single: Propel

Bazy danych a Propel
====================

Jednym z najczęstszych i trudnych zadań każdej aplikacji jest utrzymywanie i odczytywanie
informacji z baz danych. Symfony 2 Standard Edition dostarczane jest obecnie z biblioteką
Doctrine ORM. Jeżeli wolisz używać bibliotekę Propel, to integruje się ona z Symfony2 łatwo.
Aby zainstalować Propel, przeczytaj rozdział `Working With Symfony2`_ w dokumentacji Propel.

Prosty przykład: "Produkty"
---------------------------

W tym rozdziale skonfigurujemy przykładową bazę danych, tworząc obiekt ``Product``,
utrwalając go w bazie danych i pobierając go z powrotem.

.. sidebar:: Ćwiczenie przykładowego kodu

    Jeśli chcesz przećwiczyć przykład z tego rozdziału, to utwórz pakiet
    ``AcmeStoreBundle`` poleceniem:

    .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/StoreBundle

Skonfigurowanie bazy danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Zanim naprawdę zaczniemy, musisz skonfigurować informację o połączeniu ze swoją
bazą danych. Zgodnie z konwencją informacja ta zapisywana jest w pliku
``app/config/parameters.yml``:

.. code-block:: yaml
   :linenos:

    # app/config/parameters.yml
    parameters:
        database_driver:   mysql
        database_host:     localhost
        database_name:     test_project
        database_user:     root
        database_password: password
        database_charset:  UTF8

.. note::

    Definiowanie konfiguracji przez ``parameters.yml`` to tylko konwencja.
    Parametry określone w tym pliku są odnoszone do głównego pliku konfiguracyjnego,
    w którym konfigurowana jest biblioteka Propel:

Parametry zdefiniowane w ``parameters.yml`` mogą teraz być dołączone w pliku
konfiguracyjnym (``config.yml``):

.. code-block:: yaml
   :linenos:

    propel:
        dbal:
            driver:     "%database_driver%"
            user:       "%database_user%"
            password:   "%database_password%"
            dsn:        "%database_driver%:host=%database_host%;dbname=%database_name%;charset=%database_charset%"

Teraz, gdy Propel posiada informacje o bazie danych, Symfony2 może użyć tej biblioteki
do utworzenia bazy danych:

.. code-block:: bash

    $ php app/console propel:database:create

.. note::

    W tym przykładzie mamy skonfigurowane jedno połączenie z bazą danych o nazwie
    ``default``. Jeśli chcesz skonfigurować więcej połączeń, przeczytaj rozdział
    `PropelBundle Configuration`_ w dokumentacji Propel.

Utworzenie klasy modelu
~~~~~~~~~~~~~~~~~~~~~~~

W świecie Propel klasy wzorca ActiveRecord nazywane są **modelami**, ponieważ
zawierają one jakąś logikę procesów biznesowych.

.. note::

    Dla osób, które używają Symfony2 z Doctrine2, **modele** są odpowiednikami **encji**.
    Doctrine2 wykorzystuje w miejsce wzorca ActriveRecord wzorzec DataMapper.

Załóżmy, że budujemy aplikację w której powinny być wyświetlane produkty. Najpierw
więc, utwórzmy plik ``schema.xml`` wewnątrz katalogu ``Resources/config`` pakietu
``AcmeStoreBundle``:

.. code-block:: xml
   :linenos:

    <?xml version="1.0" encoding="UTF-8"?>
    <database name="default"
        namespace="Acme\StoreBundle\Model"
        defaultIdMethod="native"
    >
        <table name="product">
            <column name="id"
                type="integer"
                required="true"
                primaryKey="true"
                autoIncrement="true"
            />
            <column name="name"
                type="varchar"
                primaryString="true"
                size="100"
            />
            <column name="price"
                type="decimal"
            />
            <column name="description"
                type="longvarchar"
            />
        </table>
    </database>

Zbudowanie modelu
~~~~~~~~~~~~~~~~~

Po utworzeniu pliku ``schema.xml`` wygenerujemy z niego model uruchamiając polecenie:

.. code-block:: bash

    $ php app/console propel:model:build

Wygeneruje ono klasę każdego modelu w katalogu ``Model/`` pakietu ``AcmeStoreBundle``,
co znacznie przyśpiesza programowanie aplikacji.

Utworzenie tabel (schematów) bazy danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Mamy teraz użyteczną klase ``Product`` i jedyne co potrzebujemy, to utrwalenie jej
w bazie danych. Oczywiście, że nie mamy jeszcze w bazie danych odpowiedniej tabeli
``product``. Na szczęście Propel może automatycznie utworzyć wszystkie potrzebne
tabele dla każdego znanego modelu w bazie danych. Aby to zrobić, uruchomimy:

.. code-block:: bash

    $ php app/console propel:sql:build
    $ php app/console propel:sql:insert --force

Baza danych ma teraz w pełni funkcjonalną tabelę ``product`` z kolumnami, które
zgodne są z określonym schematem.

.. tip::

    Można uruchomić trzy ostatnie polecenia, używając jednego:
    ``php app/console propel:build --insert-sql``.

Utrwalenie obiektów w bazie danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Teraz, gdy mamy obiekt ``Product`` i odpowiadajaca mu tabelę ``product``,
gotowi jesteśmy do utrwalenia obiektu w bazie danych.  Wykorzystując kontroler,
jest to całkiem proste. Dodajmy następującą metodę fo kontrolera ``DefaultController``
pakietu::

    // src/Acme/StoreBundle/Controller/DefaultController.php

    // ...
    use Acme\StoreBundle\Model\Product;
    use Symfony\Component\HttpFoundation\Response;

    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice(19.99);
        $product->setDescription('Lorem ipsum dolor');

        $product->save();

        return new Response('Created product id '.$product->getId());
    }

W tym fragmencie kodu tworzymy instancję i posługujemy się obiektem ``$product``.
Gdy wywołamy na nim metodę ``save()``, nastąpi utrwalenie obiektu w bazie danych.
Nie potrzebujemy używać innych usług – obiekt wie jak się utrwalić.

.. note::

    Jeśli ćwiczysz niniejszy przykład, to aby zobaczyć to w działaniu, potrzebujesz
    utworzyć :doc:`trasę <routing>`, która wskazywać będzie na tą akcję.

Pobieranie obiektów z bazy danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pobranie z powrotem obiektu z bazy danych jest jeszcze łatwiejsze. Załóżmy na przykład,
że mamy skonfigurowaną trasę trasę dla wyświetlania określonego na podstawie jego
identyfikatora``id``::

    // ...
    use Acme\StoreBundle\Model\ProductQuery;

    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'No product found for id '.$id
            );
        }

        // ... zrób tu coś, jak np. przekazanie obiektu $product do szablonu
    }

Aktualizowanie obiektu
~~~~~~~~~~~~~~~~~~~~~~

Po pobraniu obiektu z Propel, zaktualizowanie go jest łatwe. Załóżmy, że mamy
trasę, która odwzorowuje identyfikator produktu na jakąś akcję aktualizowania w kontrolerze::

    // ...
    use Acme\StoreBundle\Model\ProductQuery;

    public function updateAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);

        if (!$product) {
            throw $this->createNotFoundException(
                'No product found for id '.$id
            );
        }

        $product->setName('New product name!');
        $product->save();

        return $this->redirect($this->generateUrl('homepage'));
    }

Aktualizowanie obiektu przebiega w trzech krokach:

#. pobranie obiektu z Propel (linie 6 - 13);
#. zmodyfikowanie obiektu (linia 15);
#. zapisanie obiektu (linia 16).

Usuwanie obiektu
~~~~~~~~~~~~~~~~

Usuwanie obiektu jest bardzo podobne do aktualizacji, ale wymagana wywołania na
obiekcie metody ``delete()``::

    $product->delete();

Zapytania do obiektów
---------------------

Propel udostępnia wygenerowane klasy ``Query`` w celu uruchamiania zarówno podstawowych
jak i złożonych zapytań bez jakiegokolwiek wysiłku::

    \Acme\StoreBundle\Model\ProductQuery::create()->findPk($id);

    \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByName('Foo')
        ->findOne();

Przyjmijmy, że chcemy zapytać się o produkty, których koszt jest większy niż  19.99,
posortowane od najtańszych do najdroższych. W kontrolerze napiszemy następujący kod::

    $products = \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByPrice(array('min' => 19.99))
        ->orderByPrice()
        ->find();

W jednej linii można pobrać produkty w sposób zorientowany obiektowo. Nie trzeba
tracić czasu na zapytania SQL lub cokolwiek innego - Symfony2 oferuje programowanie
w pełni zorientowane obiektowo a Propel respektuje tą samą filozofię dostarczając
bardzo dobrą warstwę abstrakcji.

Jeśli chce się użyć ponownie tego samego zapytania, to można dodać własne metody
do klasy ``ProductQuery``::

    // src/Acme/StoreBundle/Model/ProductQuery.php
    class ProductQuery extends BaseProductQuery
    {
        public function filterByExpensivePrice()
        {
            return $this
                ->filterByPrice(array('min' => 1000));
        }
    }

Warto wiedzieć, że Propel generuje dużo metod i proste ``findAllOrderedByName()``
może zostać nadpisane bez wysiłku::

    \Acme\StoreBundle\Model\ProductQuery::create()
        ->orderByName()
        ->find();

Relacje (powiązania)
--------------------

Załóżmy, że produkty w aplikacji należą dokładnie do jednej "kategorii".
W tym przypadku potrzebny będzie obiekt ``Category`` i sposób na odniesienie
obiektu ``Product`` do obiektu ``Category``.

Rozpoczniemy dodając definicję  ``category`` w naszym schemacie  ``schema.xml``:

.. code-block:: xml
   :linenos:

    <database name="default" namespace="Acme\StoreBundle\Model" defaultIdMethod="native">
        <table name="product">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
            <column name="price" type="decimal" />
            <column name="description" type="longvarchar" />

            <column name="category_id" type="integer" />
            <foreign-key foreignTable="category">
                <reference local="category_id" foreign="id" />
            </foreign-key>
        </table>

        <table name="category">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
       </table>
    </database>

Utwórzmy klasy:

.. code-block:: bash

    $ php app/console propel:model:build

Przyjmijmy, że w bazie danych mamy już produkty i nie chcemy stracić tych danych
podczas aktualizacji. Dzięki migracjom Propel zaktualizuje bazę danych bez utraty
istniejących danych.

.. code-block:: bash

    $ php app/console propel:migration:generate-diff
    $ php app/console propel:migration:migrate

Nasza baza danych została zaktualizowana, możemy dalej pisać swoją aplikację.

Zapisywanie powiązanych obiektów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Teraz wypróbujmy ten kod w działaniu. Przyjmijmy, że mamy następujący kod kontrolera::

    // ...
    use Acme\StoreBundle\Model\Category;
    use Acme\StoreBundle\Model\Product;
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

            // save the whole
            $product->save();

            return new Response(
                'Created product id: '.$product->getId().' and category id: '.$category->getId()
            );
        }
    }

Teraz pojedynczy wiersz jest dodawany do obu tabel ``category`` i ``product``.
Kolumna ``product.category_id`` dla nowego produktu jest ustawiana na identyfikator
nowej kategorii. Propel sam zarządza utrzymaniem tej relacji.

Pobieranie powiązanych objektów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Gdy zachodzi potrzeba pobrania powiązanych obiektów, działanie wygląda tak jak
miało to miejsce poprzednio. Najpierw trzeba pobrać obiekt ``$product``
a następnie uzyskać dostęp do powiązanego obiektu ``Category``::

    // ...
    use Acme\StoreBundle\Model\ProductQuery;

    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->joinWithCategory()
            ->findPk($id);

        $categoryName = $product->getCategory()->getName();

        // ...
    }

Proszę zwrócić uwagę na to, że w powyższym przykładzie napisane jest tylko zapytanie.

Więcej informacji o powiązaniach
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Więcej informacji o relacjach znaleźć można w rozdziale `Relationships`_
w dokumentacji Propel.

Wywołania zwrotne cyklu życia
-----------------------------

Czasem zachodzi potrzeba wykonania akcji tuż przed lub po tym jak obiekt jest
wstawiany, aktualizowany lub usuwany.  Te typy akcji nazywane są wywołaniami
zwrotnym "cyklu życia" lub "hakami", ponieważ są to metody wywołań zwrotnych,
które wykonuje się w różnych etapach istnienia obiektu (np. obiekt jest wstawiany,
aktualizowany, usuwany itd.).

Aby dodać hak, wystarczy dodać nową metodę do klasy obiektu::

    // src/Acme/StoreBundle/Model/Product.php

    // ...
    class Product extends BaseProduct
    {
        public function preInsert(\PropelPDO $con = null)
        {
            // do something before the object is inserted
        }
    }

Propel udustępnia następujące haki:

* ``preInsert()`` wykonanie kodu przed wstawieniem nowego obiektu
* ``postInsert()`` wykonanie kodu po wstawieniu nowego obiektu
* ``preUpdate()`` wykonanie kodu przed zaktualizowaniem istniejącego obiektu
* ``postUpdate()`` wykonanie kodu po zaktualizowanii istniejącego obiektu
* ``preSave()`` wykonanie kodu przed zapisaniem obiektu (nowego lub istniejącego)
* ``postSave()`` wykonanie kodu po zapisaniu obiektu (nowego lub istniejącego)
* ``preDelete()`` wykonanie kodu przed usunięciem obiektu
* ``postDelete()`` wykonanie kodu po usunięciu obiektu


Zachowania
----------

W Symfony2 działają  wszystkie zachowania udostępnione w pakietach Propel.
Aby uzyskać więcej informacji o zachowaniach Propel, proszę zapoznać się z rozdziałem
`Behaviors Reference`_ dokumentacji Propel.

Polecenia
---------

Proszę przeczytać specjalny rozdział poświęcony `poleceniom Propel w Symfony2`_.

.. _`Working With Symfony2`: http://propelorm.org/cookbook/symfony2/working-with-symfony2.html#installation
.. _`PropelBundle Configuration`: http://propelorm.org/cookbook/symfony2/working-with-symfony2.html#configuration
.. _`Relationships`: http://propelorm.org/documentation/04-relationships.html
.. _`Behaviors Reference`: http://propelorm.org/documentation/#behaviors_reference
.. _`poleceniom Propel w Symfony2`: http://propelorm.org/cookbook/symfony2/working-with-symfony2#the_commands
