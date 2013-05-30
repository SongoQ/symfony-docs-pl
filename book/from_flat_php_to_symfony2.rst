.. highlight:: php
   :linenothreshold: 2

Symfony2 versus zwykły PHP
==========================

**Dlaczego Symfony2 jest lepszy niż otworzenie pliku i pisanie zwykłego PHP?**

Jeśli nigdy jeszcze nie używałeś frameworka PHP, nie jesteś zaznajomiony z filozofią
MVC lub po prostu zastanawiasz się dlaczego tyle rozgłosu jest wokół Symfony2, to
ten rozdział jest dla Ciebie. Zamiast wmawiać Ci, że Symfony2 pozwala tworzyć
oprogramowanie szybciej i lepiej niż przez pisanie skryptów w zwykłym PHP, pokażemy
Ci to na przykładach.

W tym rozdziale napiszemy prostą aplikację w zwykłym PHP a następnie przerobimy ją
tak, aby była lepiej zorganizowana. Będziemy podróżować w czasie, obserwując powody
dla których tworzenie stron internetowych ewaluowało na przestrzeni ostatnich lat
i gdzie jest teraz.

Na koniec zobaczysz jak Symfony2 może uwolnić Cię od prozaicznych zadań i pozwala
uzyskać kontrolę nad kodem.

Prosty blog w zwykłym PHP
-------------------------

W tym rozdziale zbudujemy symboliczną aplikacje bloga używając tylko zwykłego PHP.
Aby rozpocząć, utworzymy plik, który będzie wyświetlał wpisy bloga, które zostały
utrwalona w bazie danych. Kod napisany w zwykłym PHP jest szybki, lecz pogmatwany:

.. code-block:: html+php
   :linenos:

    <?php
    // index.php
    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);
    ?>

    <!DOCTYPE html>
    <html>
        <head>
            <title>List of Posts</title>
        </head>
        <body>
            <h1>List of Posts</h1>
            <ul>
                <?php while ($row = mysql_fetch_assoc($result)): ?>
                <li>
                    <a href="/show.php?id=<?php echo $row['id'] ?>">
                        <?php echo $row['title'] ?>
                    </a>
                </li>
                <?php endwhile; ?>
            </ul>
        </body>
    </html>

    <?php
    mysql_close($link);
    ?>

Jest to szybkie w napisaniu i szybkie w wykonaniu, ale co będzie, gdy aplikacja
zacznie się rozrastać – jej utrzymanie stanie się niemożliwe. Z tym kodem wiąże
się kilka problemów, które należy wziąć pod uwagę:

* **Brak sprawdzania błędów**. Co gdy połączenie z bazą danych nie uda się?

* **Zła organizacja**. Jeżeli aplikacja zacznie się rozrastać, to kod zawarty w
   jednym pliku będzie nie do utrzymania. Gdzie należy umieścić kod do obsługi
   formularzy i ich zgłoszeń? Jak można sprawdzać dane? Gdzie powinien się
   znajdować kod do wysyłania wiadomości e-mail?

* **Trudno wielokrotnie wykorzystywać fragmenty kodu**. Ponieważ wszystko jest w
   jednym pliku, to nie ma możliwości ponownego wykorzystania fragmentów kodu
   aplikacji dla innych „stron” blogu.

.. note::

     Innym problemem, tutaj nie wymienionym, jest fakt, że baza danych
     jest w tej aplikacji ograniczona do MySQL. Choć o tym tutaj nie mówimy,
     to Symfony2 w pełni integruje `Doctrine`_, bibliotekę dostarczającą
     abstakcyjną warstwę dostępu do baz danych i `mapowanie obiektowo-relacyjne`_
     (*ang. Object-Relational Mapping - ORM*).

Bierzmy się więc do pracy nad rozwiązaniem tych problemów.

Odizolowanie prezentacji
~~~~~~~~~~~~~~~~~~~~~~~~

Kod może natychmiast zyskać po rozdzieleniu „logiki” aplikacji od kodu HTML
wykonującego „prezentację”:

.. code-block:: html+php
   :linenos:

    <?php
    // index.php
    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    $posts = array();
    while ($row = mysql_fetch_assoc($result)) {
        $posts[] = $row;
    }

    mysql_close($link);

    // dołączenie kodu HTML warstwy prezentacji  
    require 'templates/list.php';

Kod HTML jest teraz przechowywany w odrębnym pliku (``templates/list.php``), który
jest przede wszystkim plikiem HTML używającym składni „szablonopodobnej” PHP:

.. code-block:: html+php
   :linenos:

    <!DOCTYPE html>
    <html>
        <head>
            <title>Wykaz wpisów</title>
        </head>
        <body>
            <h1>Wykaz wpisów</h1>
            <ul>
                <?php foreach ($posts as $post): ?>
                <li>
                    <a href="/read?id=<?php echo $post['id'] ?>">
                        <?php echo $post['title'] ?>
                    </a>
                </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>

Plik, który przechowuje całą logikę aplikacji (``index.php``) jest umownie nazywany
"kontrolerem". Termin :term:`kontroler` jest słowem o którym dużo usłyszysz,
niezależnie od języka czy frameworka jakiego będziesz używał. Odnosi się to tylko
do obszaru kodu, który przetwarza dane wejściowe i przygotowuje odpowiedź.

W naszym przypadku, kontroler przygotowuje dane z bazy danych a następnie dołącza
szablon w celu prezentacji danych. Po wydzieleniu kontrolera można łatwo zmienić
sam plik szablonu, jeśli jest to potrzebne do wygenerowania wpisów bloga w jakimś
innym formacie (np. ``list.json.php`` dla formatu JSON ).

Odizolowanie logiki aplikacji (domeny)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dotychczas nasza aplikacja zawierała tylko jedną stronę. Ale co, gdy potrzebna
będzie druga strona używająca tego samego połączenia z bazą danych, a nawet tej
samej tabeli wpisów bloga? Przeorganizujmy kod tak, aby funkcje podstawowego
zachowania i dostępu do bazy danych zostały rozdzielone i te drugie zostały
przeniesione do pliku o nazwie ``model.php``:

.. code-block:: html+php
   :linenos:

    <?php
    // model.php
    function open_database_connection()
    {
        $link = mysql_connect('localhost', 'myuser', 'mypassword');
        mysql_select_db('blog_db', $link);

        return $link;
    }

    function close_database_connection($link)
    {
        mysql_close($link);
    }

    function get_all_posts()
    {
        $link = open_database_connection();

        $result = mysql_query('SELECT id, title FROM post', $link);
        $posts = array();
        while ($row = mysql_fetch_assoc($result)) {
            $posts[] = $row;
        }
        close_database_connection($link);

        return $posts;
    }

.. tip::

   Użyliśmy nazwy pliku ``model.php`` ponieważ logika aplikacji i dostępem do
   bazy danych jest częścią kodu aplikacji tradycyjnie nazywaną warstwą "modelu".
   W dobrze zorganizowanej aplikacji większość kodu reprezentującego "logikę biznesową"
   powinna znajdować się w modelu (a nie w kontrolerze). W przeciwieństwie do tego
   przykładu, tylko część modelu (lub nic) faktycznie dotyczy dostępu do bazy danych.

Kontroler (``index.php``) jest teraz bardzo prosty:

.. code-block:: html+php
   :linenos:

    <?php
    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';

Teraz jedynym zadaniem kontrolera jest pobranie danych z modelu i wywołanie szablonu
w celu wygenerowania tych danych. Jest to bardzo prosty przykład wzorca
**model-widok-kontroler** (*ang. model-view-controller, MVC*).

Odizolowanie układu
~~~~~~~~~~~~~~~~~~~

W tym momencie aplikacja została rozdzielona na trzy odrębne części, oferujących
różne zalety i możliwości ponownego wykorzystania niemal wszystkiego na różnych
stronach.

Tylko część kodu, która nie może być ponownie wykorzystana, to układ stron.
Poprawmy to przez utworzenie nowego pliku ``layout.php``:

.. code-block:: html+php
   :linenos:

    <!-- templates/layout.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <title><?php echo $title ?></title>
        </head>
        <body>
            <?php echo $content ?>
        </body>
    </html>

Szablon (``templates/list.php``) może teraz zostać uproszczony do "rozszerzenia"
układu:

.. code-block:: html+php
   :linenos:

    <?php $title = 'List of Posts' ?>

    <?php ob_start() ?>
        <h1>List of Posts</h1>
        <ul>
            <?php foreach ($posts as $post): ?>
            <li>
                <a href="/read?id=<?php echo $post['id'] ?>">
                    <?php echo $post['title'] ?>
                </a>
            </li>
            <?php endforeach; ?>
        </ul>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

Masz teraz wprowadzoną metodologię, która umożliwia ponowne wykorzystanie układu.
Niestety, aby to osiągnąć, zmuszony jesteś do użycia w szablonie kilku kiepskich
funkcji PHP (``ob_start()``, ``ob_get_clean()``). Symfony2 wykorzystuje komponent
``Templating``, umożliwiający osiągnąć ten cel w sposób prosty i przejrzysty.
Zobaczymy to już wkrótce.

Dodanie strony "show" blogu
---------------------------

Strona blogu "list" została teraz przekształcona tak, aby kod był zorganizowany
lepiej i mógł być wielokrotnie wykorzystywany. Aby to udowodnić dodamy stronę
blogu "show", wyświetlającą pojedynczy wpis blogu, identyfikowany przez parametr
zapytania ``id``.

Aby rozpocząć, utworzymy nową funkcję w pliku ``model.php``, która pobiera
pojedynczy wpis blogu na podstawie parametru id::

    // model.php
    function get_post_by_id($id)
    {
        $link = open_database_connection();

        $id = intval($id);
        $query = 'SELECT date, title, body FROM post WHERE id = '.$id;
        $result = mysql_query($query);
        $row = mysql_fetch_assoc($result);

        close_database_connection($link);

        return $row;
    }

Następnie utworzymy nowy plik ``show.php`` - kontroler dla nowej strony:

.. code-block:: html+php
   :linenos:

    <?php
    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

Na koniec, utwórzmy nowy plik szablonu, ``templates/show.php``, aby wygenerować
pojedynczy wpis blogu:

.. code-block:: html+php
   :linenos:

    <?php $title = $post['title'] ?>

    <?php ob_start() ?>
        <h1><?php echo $post['title'] ?></h1>

        <div class="date"><?php echo $post['date'] ?></div>
        <div class="body">
            <?php echo $post['body'] ?>
        </div>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>

Utworzenie drugiej strony jest teraz bardzo łatwe a kod nie jest powielany.
Pomimo tego, strona ta stwarza dalej kilka problemów, które rozwiązuje 
framework. Na przykład, brak lub nieprawidłowy parametr zapytania ``id`` spowoduje
załamanie sie strony ("biały ekran"). Byłoby lepiej, gdyby spowodowało to wygenerowanie
strony błedu 404, ale nie może być to tak łatwo zrobione. Gorzej, gdybyś zapomniał
przekształcić parametr ``id`` za pomocą funkcji ``intval()`` - wówczas cała baza danych
zostałaby narażona na atak wstrzyknięcia SQL

Innym ważnym problemem jest to, że każdy plik kontrolera musi dołączać plik
``model.php``. Co jeśli każdy plik kontrolera nagle będzie potrzebował dołączyć
dodatkowy plik lub wykonać inne zadanie globalne (np. wymusić zabezpieczenie)?
W obecnym stanie, taki kod będzie musiał być dodany do każdego pliku kontrolera.
Jeżeli zapomni się coś dodać w jakimś pliku, to powstanie następny problem, miejmy
nadzieję, że nie dotyczy to bezpieczeństwa ...

Lekarstwem "kontroler wejścia"
------------------------------

Rozwiązanie jest zastosowanie *kontrolera wejściowego* -  pojedynczego pliku PHP,
w którym przetwarzane są wszystkie żądania HTTP. Przy zastosowaniu kontrolera
wejściowego nieco zmieniają się adresy URI, ale zaczynają się bardziej elastycznie:

.. code-block:: text
   :linenos:

    Without a front controller
    /index.php          => Blog post list page (index.php executed)
    /show.php           => Blog post show page (show.php executed)

    With index.php as the front controller
    /index.php          => Blog post list page (index.php executed)
    /index.php/show     => Blog post show page (index.php executed)

.. tip::
    The ``index.php`` portion of the URI can be removed if using Apache
    rewrite rules (or equivalent). In that case, the resulting URI of the
    blog show page would be simply ``/show``.

Podczas korzystania z kontrolera wejściowego pojedynczy plik PHP (w naszym
przypadku ``index.php``) przetworzy każde żądanie HTTP. W celu wyświetlenia strony
"show” żądany jest zasób ``/index.php/show`` a w rzeczywistości wykonywany jest
plik ``index.php``, który jest teraz odpowiedzialny za wewnętrzne kierowanie żądań
na podstawie pełnego adresu URI. Jak widzisz, kontroler wejścia jest bardzo
silnym narzędziem.

Stworzenie kontrolera wejścia
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Mamy zamiar zrobić duży krok w rozbudowie aplikacji. Przy pomocy jednego pliku
będziemy obsługiwać wszystkie żądania, centralizując takie rzeczy jak obsługa
bezpieczeństwa, ładowanie i konfigurację trasowanie. Plik ``index.php`` musi teraz
być wystarczająco inteligentny, aby wygenerowac stronę wpisów bloga lub stronę
wpisu kierując się adresem URI:

.. code-block:: html+php
   :linenos:

    <?php
    // index.php

    // load and initialize any global libraries
    require_once 'model.php';
    require_once 'controllers.php';

    // route the request internally
    $uri = $_SERVER['REQUEST_URI'];
    if ('/index.php' == $uri) {
        list_action();
    } elseif ('/index.php/show' == $uri && isset($_GET['id'])) {
        show_action($_GET['id']);
    } else {
        header('Status: 404 Not Found');
        echo '<html><body><h1>Page Not Found</h1></body></html>';
    }

W celach organizacyjnych oba kontrolery (dawniej ``index.php`` i ``show.php``)
są teraz funkcjami PHP i zostały przeniesione do odrębnego pliku ``controllers.php``:

.. code-block:: php
   :linenos:

    function list_action()
    {
        $posts = get_all_posts();
        require 'templates/list.php';
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        require 'templates/show.php';
    }

Plik ``index.php`` jako kontroler wejścia przybrał teraz całkiem nową rolę,
która polega na załadowaniu podstawowych bibliotek i trasowaniu aplikacji, tak
aby wywołany został jeden z dwóch kontrolerów (funkcje ``list_action()``
i ``show_action()``). Aktualnie nasz kontroler wejścia zaczyna wyglądać i
działać jak mechanizm Symfony2 do obsługi i trasowania żądań.

.. tip::

   Inną zaletą kontrolera wejściowego jest możliwość stosowania elastycznych
   adresów URL. Proszę zauważyć, że adres URL do strony wpisu bloga może być
   zmieniony z ``/show`` na ``/read`` tylko przez zmianę kodu w jednym miejscu.
   Przedtem musiał by być zmieniony cały plik aby można było zmienić nazwę strony.
   W Symfony2 adresy URL są bardziej elastyczne.

Do teraz nasza aplikacja ewoluowała z pojedynczego pliku PHP w strukturę, która
jest zorganizowana i umożliwia wielokrotne wykorzystanie kodu. Powinniśmy być
szczęśliwi, ale jeszcze daleko do zadowolenia. Na przykład, system „trasowania”
jest niestabilny a strona wykazu wpisów bloga (dostępna przez adres ``/index.php``)
powinna być również dostępna przez adres / (jeżeli dodana jest reguła rewrite Apache).
Ponadto, zamiast tworzyć blog, dużo czasu tracimy na pracę z "architekturą" kodu
(np. trasowanie, wywoływanie kontrolerów, szablony itd.). Jeszcze więcej czasu
trzeba będzie przeznaczyć na obsługę zgłoszeń formularzy, walidację danych
wejściowych, rejestrowanie i bezpieczeństwo. Czy nie warto mieć gotowe rozwiązanie
tych rutynowych problemów?

Dodanie odrobiny Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~

Na ratunek - Symfony2. Nim zaczniesz używać Symfony2, musisz się upewnić, czy
PHP wie jak znaleźć klasy Symfony2. Uzyskuje się to poprzez autoloadera, który
jest dostarczany przez Symfony. Autoloader jest narzędziem pozwalającym na
rozpoczęcie używania klas PHP bez konieczności jawnego dołączania pliku
zawierającego klasę.

W głównym katalogu naszej aplikacji utwórz plik ``composer.json`` z następującą
zawartością:

.. code-block:: json
   :linenos:

    {
        "require": {
            "symfony/symfony": "2.2.*"
        },
        "autoload": {
            "files": ["model.php","controllers.php"]
        }
    }
    
Następnie `pobierz Composer`_ i następnie uruchom następujące polecenie, które
załaduje Symfony do katalogu ``vendor/``:

.. code-block:: bash

    $ php composer.phar install

Wraz z pobraniem zależności Composer generuje plik ``vendor/autoload.php``,
którego zadaniem jest automatyczne załadowanie wszystkich plików Symfony Framework,
jak również plików wymienionych w sekcji ``autoload`` pliku ``composer.json``.

Filozofią rdzenia Symfony jest przekonanie, że głównym zadaniem aplikacji jest
interpretacja każdego żądania i zwracanie odpowiedzi. W tym celu Symfony2 dostarcza
klasy :class:`Symfony\\Component\\HttpFoundation\\Request` jak i
:class:`Symfony\\Component\\HttpFoundation\\Response`.
Klasy te są obiektowo zorientowaną reprezentacją surowego żądania HTTP, które ma
być przetworzone, oraz odpowiedzi, która ma być zwrócona. Wykorzystajmy te obiekty
do poprawienia naszego blogu:

.. code-block:: html+php
   :linenos:

    <?php
    // index.php
    require_once 'vendor/autoload.php';

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();

    $uri = $request->getPathInfo();
    if ('/' == $uri) {
        $response = list_action();
    } elseif ('/show' == $uri && $request->query->has('id')) {
        $response = show_action($request->query->get('id'));
    } else {
        $html = '<html><body><h1>Page Not Found</h1></body></html>';
        $response = new Response($html, 404);
    }

    // echo the headers and send the response
    $response->send();

Kontrolery są teraz odpowiedzialne za zwrócenie obiektu ``Response``.
Aby to ułatwić, można dodać nową funkcję ``render_template()``, która nawiasem
mówiąc, działa trochę jak silnik szablonowania Symfony2:

.. code-block:: php
   :linenos:

    // controllers.php
    use Symfony\Component\HttpFoundation\Response;

    function list_action()
    {
        $posts = get_all_posts();
        $html = render_template('templates/list.php', array('posts' => $posts));

        return new Response($html);
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        $html = render_template('templates/show.php', array('post' => $post));

        return new Response($html);
    }

    // helper function to render templates
    function render_template($path, array $args)
    {
        extract($args);
        ob_start();
        require $path;
        $html = ob_get_clean();

        return $html;
    }

Po wprowadzenie niewielkiej części Symfony2, aplikacja stała się bardziej elastyczna
i niezawodna. Klasa ``Request`` zapewnia niezawodny sposób dostępu do informacji
o żądaniu HTTP. Konkretniej, metoda ``getPathInfo()`` zwraca oczyszczony adres
URI (zawsze zwrane jest  ``/show a nigdy`` ``/index.php/show``). Tak więc, nawet
gdy użytkownik zażąda ``/index.php/show``, to aplikacja w sposób inteligentny
skieruje żądanie do metody ``show_action()``.

Obiekt ``Response`` daje elastyczność przy konstruowaniu odpowiedzi HTTP, dzięki
czemu nagłówki HTTP i zawartość są dodawane poprzez interfejs obiektowo zorientowany.
Chociaż odpowiedzi w naszej aplikacji są proste, to uzyskana teraz elastyczność
zacznie procentować, gdy aplikacja zacznie się rozrastać.

Prosta aplikacja w Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nasz blog przebył długą drogę, ale nadal zawiera wiele kodu jak dla tak prostej
aplikacji. Po drodze, wykonaliśmy prosty system trasowania i metodę stosującą
funkcje ``ob_start()`` i ``get_clean()`` do wygenerowania szablonu.
Jeśli z jakiegoś powodu chcesz kontynuować budowę tego "szkieletu", można
przynajmniej posłużyć się samodzielnymi komponentami Symfony, takimi jak
`Routing`_ i `Templating`_, które rozwiązują wiele problemów.

Zamiast ponownie rozwiązywać już rozwiązane problemy, możesz pozwolić aby Symfony2
zajęło się tymi problemami. Oto przykładowa aplikacja, tym razem zbudowana w całości
w Symfony2::

    // src/Acme/BlogBundle/Controller/BlogController.php
    namespace Acme\BlogBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $posts = $this->get('doctrine')->getManager()
                ->createQuery('SELECT p FROM AcmeBlogBundle:Post p')
                ->execute();

            return $this->render(
                'AcmeBlogBundle:Blog:list.html.php',
                array('posts' => $posts)
            );
        }

        public function showAction($id)
        {
            $post = $this->get('doctrine')
                ->getManager()
                ->getRepository('AcmeBlogBundle:Post')
                ->find($id)
            ;

            if (!$post) {
                // cause the 404 page not found to be displayed
                throw $this->createNotFoundException();
            }

            return $this->render(
                'AcmeBlogBundle:Blog:show.html.php',
                array('post' => $post)
            );
        }
    }

Oba kontrolery są nadal lekkie. Każdy wykorzystuje bibliotekę :doc:`doctrine`
do pobierania obiektów z bazy danych oraz komponent ``Templating`` do wygenerowania
szablonu i zwracania obiektu Response. Szablon wykazu wpisów na blogu jest teraz
nieco prostszy:

.. code-block:: html+php
   :linenos:

    <!-- src/Acme/BlogBundle/Resources/views/Blog/list.html.php -->
    <?php $view->extend('::layout.html.php') ?>

    <?php $view['slots']->set('title', 'List of Posts') ?>

    <h1>List of Posts</h1>
    <ul>
        <?php foreach ($posts as $post): ?>
        <li>
            <a href="<?php echo $view['router']->generate(
                'blog_show',
                array('id' => $post->getId())
            ) ?>">
                <?php echo $post->getTitle() ?>
            </a>
        </li>
        <?php endforeach; ?>
    </ul>

Układ jest niemal identyczny:

.. code-block:: html+php
   :linenos:

    <!-- app/Resources/views/layout.html.php -->
    <!DOCTYPE html>
    <html>
        <head>
            <title><?php echo $view['slots']->output(
                'title',
                'Default title'
            ) ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    Szablon wpisu na blogu pozostawiamy jako wzorzec, jako że utworzenie na jego
    podstawie szablonu wykazu wpisów na blogu będzie trywialne.

Kiedy uruchamia się silnik Symfony2 (o nazwie ``Kernel``), potrzebuje on mapy,
tak aby wiedzieć jaki kontroler należy wykonać na podstawie informacji z żądania
HTTP. Informacje te są dostarczane w czytelnej formie przez mapę konfiguracji
trasowania:

.. code-block:: yaml
   :linenos:

    # app/config/routing.yml
    blog_list:
        path:     /blog
        defaults: { _controller: AcmeBlogBundle:Blog:list }

    blog_show:
        path:     /blog/show/{id}
        defaults: { _controller: AcmeBlogBundle:Blog:show }

Teraz Symfony2 obsługuje wszystkie prozaiczne zadania, kontroler wejścia jest
dziecinnie prosty. Ponieważ to nie tak mało, nie musisz go zmieniać po utworzeniu
(a jeśli używasz dystrybucji Symfony2, to nawet nie trzeba go tworzyć)::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

Jedynym zadaniem kontrolera wejściowego jest inicjacja silnika (``Kernela``)
 Symfony2 i przekazaniu mu do przetworzenia obiektu ``Request``.
 Rdzeń Symfony2 następnie używa mapy trasowania do ustalenia kontrolera, który
 należy wywołać. Tak jak wcześniej, metoda kontrolera jest odpowiedzialna za
 zwrócenie w efekcie końcowym obiektu ``Response``. Tam naprawdę niewiele tego.

Otwórz :ref:`diagram przepływu żądania<request-flow-figure>`, aby obejrzeć
wizualną prezentację tego, jak Symfony2 obsługuje żądanie.

W czym pomógł Symfony2
~~~~~~~~~~~~~~~~~~~~~~

W dalszym rozdziale dowiesz się więcej o tym, jak działa każda część Symfony
i jaka jest zalecana organizacja projektu. Teraz zobaczmy, jak migracja blogu,
od zwykłego PHP do Symfony2, ułatwiła nam życie:

* Aplikacja ma teraz jasny i konsekwentnie zorganizowany kod (choć Symfony nie
  wymusza tego). Kod nabywa zdolności do wielokrotnego wykorzystania i pozwala
  programistom na zwiększenie produktywności poprzez przyśpieszenie tworzenia
  projektu;

* Programista może cały wysiłek poświecić tworzeniu aplikacji. Nie musi on tworzyć
  ani utrzymywać narzędzi niskiego poziomu, takich jak
  :ref:`automatyczne ładowanie<autoloading-introduction-sidebar>`,
  :doc:`trasowanie<routing>` czy renderowanie w :doc:`kontrolerach<controller>`;

* Symfony2 daje dostęp do otwartych narzędzi, takich jak Doctrine i komponentów
  szablonowania, bezpieczeństwa, formularzy, walidacji i tłumaczeń (by wymienić
  tylko kilka);

* Dzięki komponentowi ``Routing`` aplikacja posiada teraz **przyjazne, w pełni
  elastyczne adresy URL**;

* Architektura Symfony2 ukierunkowana na HTTP daje dostęp do zaawansowanych narzędzi,
  takich jak buforowanie HTTP wspierane przez wewnętrzną pamięć podręczną HTTP
  Symfony2 lub bardziej zaawansowane narzędzia, takie jak ``Varnish``. Wszystko o
  :doc:`buforowaniu<http_cache>` jest opisane w dalszej części podręcznika.

Być może najważniejszym pożytkiem przy używaniu Symfony2 jest dostęp do całego
zestawu wysokiej jakości narzędzi o otwartym kodzie, opracowanych przez społeczność
Symfony2. Dobry wybór społecznościowych narzędzi Symfony2 można znaleźć na stronie
`KnpBundles.com`_.

Lepsze szablony
---------------

Jeśli zdecydujesz się na używanie Symfony2, to jest on wyposażony w silnik szablonów
o nazwie `Twig`_, który sprawia, że szablony są szybsze w pisaniu i łatwiejsze w
czytaniu. Oznacza to też, że nasza przykładowa aplikacja może zawierać jeszcze
mniej kodu. Dla przykładu przekształćmy szablon wykazu wpisów bloga na szablon
napisany w Twigu:

.. code-block:: html+jinja
   :linenos:

    {# src/Acme/BlogBundle/Resources/views/Blog/list.html.twig #}
    {% extends "::layout.html.twig" %}

    {% block title %}List of Posts{% endblock %}

    {% block body %}
        <h1>List of Posts</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', {'id': post.id}) }}">
                    {{ post.title }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

Odpowiedni szablon ``layout.html.twig`` jest równie prosty:

.. code-block:: html+jinja
   :linenos:

    {# app/Resources/views/layout.html.twig #}
    <!DOCTYPE html>
    <html>
        <head>
            <title>{% block title %}Default title{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Twig jest dobrze obsługiwany przez Symfony2, podobnie jak szablony PHP. Twig
zostanie omówiony dokładniej w dalszej części podręcznika. Więcej informacji
można znaleźć w rozdziale ":doc:`templating`".

Dowiedz się więcej w Receptariuszu
----------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`pobierz Composer`: http://getcomposer.org/download/
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`KnpBundles.com`: http://knpbundles.com/
.. _`Twig`: http://twig.sensiolabs.org
.. _`Varnish`: https://www.varnish-cache.org/
.. _`PHPUnit`: http://www.phpunit.de
.. _`Doctrine`: http://www.doctrine-project.org/
.. _`mapowanie obiektowo-relacyjne`: http://pl.wikipedia.org/wiki/Mapowanie_obiektowo-relacyjne
