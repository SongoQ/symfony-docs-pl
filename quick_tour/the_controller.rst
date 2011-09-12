Kontroler
=========

Wciąż z nami po pierwszych dwóch częściach?
Zaczynasz się uzależniać os Symfony2! Bez zbędnych ceregieli, zobacz
co kontrolery potrafią zrobić dla Ciebie.

Formaty Używania
----------------

Obecnie, aplikacja webowa powinna być w stanie dostarczyć więcej niż tylko
strony HTML. Od XML dla RSS lub Usług Internetowych (Web Services), do
JSON dla zapytań Ajaxowych, istnieje wiele różnych formatów do wyboru.
Symfony2 w prosty sposób wspiera te formaty. Ulepsz routing poprzez dodanie
domyślnej wartości ``xml`` dla zmiennej ``_format``::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}", defaults={"_format"="xml"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Dzięki wykorzystaniu formatu zapytania (zdefiniowanego przez wartość ``_format``),
Symfony2 automatycznie zaznacza odpowiedni szablon, w tym przypadku ``hello.xml.twig``:

.. code-block:: xml+php

    <!-- src/Acme/DemoBundle/Resources/views/Demo/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

Wszystko w temacie. Dla standardowych formatów, Symfony2 będzie także
automatycznie dobierał najlepszy nagłówek ``Content-Type`` dla odpowiedzi
(response). Jeśli chcesz mieć możliwość obsłużenia kilku formatów w jednej akcji,
użyj rozgranicznika w wzorcu routingu::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}.{_format}", defaults={"_format"="html"}, requirements={"_format"="html|xml|json"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Kontroler może być teraz wywoływany przez URL ``/demo/hello/Fabien.xml`` lub
``/demo/hello/Fabien.json``.

Wpis ``requirements`` definiuje wyrażenie regularne do którego rozgraniczniki muszą pasować.
W tym przykładzie, jeśli spróbujesz dostać się do ``/demo/hello/Fabien.js``, dostaniesz
w odpowiedzi błąd HTTP 404, ponieważ nie pasuje do wymaganej opcji ``_format``.

Przekierowania (Redirecting) oraz Przekazania (Forwarding)
----------------------------------------------------------

Jeśli chcesz przekierować użytkownika do innej strony, użyj metody ``redirect()``::

    return $this->redirect($this->generateUrl('_demo_hello', array('name' => 'Lucas')));

Metoda ``generateUrl()`` jest identyczna z funkcją ``path()`` którą używaliśmy w szablonach.
Przyjmuje nazwe routingu oraz tablicę parametrów jako atrybuty, oraz zwraca przygotowany
przyjazny URL.

Możesz także w łatwy sposób przekazać akcję do innej akcji używając metodę ``forward()``.
Wewnętrznie Symfony2 wykonuje "pod-zapytanie", oraz zwraca obiekt ``Response`` z tego
pod-zapytania::

    $response = $this->forward('AcmeDemoBundle:Hello:fancy', array('name' => $name, 'color' => 'green'));

    // zrób coś z obiektem odpowiedzi lub też zwróć go bezpośrednio

Pobieranie informacji z Zapytania (Request)
-------------------------------------------

Poza dostępem do wartości parametrów routingu, kontroler ma także dostęp do
obiektu ``Request``::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // czy jest to zapytanie Ajax?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // pobranie parametru $_GET

    $request->request->get('page'); // pobranie parametru $_POST

W szablonie, możesz także uzyskać dostęp do biektu ``Request`` poprzez
zmienną ``app.request``:

.. code-block:: html+jinja

    {{ app.request.query.get('page') }}

    {{ app.request.parameter('page') }}

Trzymanie Danych w Sesji
------------------------

Nawet jeśli HTTP jest protokołem bezstanowym, Symfony2 zapewnia miły
obiekt sesji reprezentujący klienta (może to być prawdziwa osoba używająca
przeglądarki, bot, lub też web service). Pomiędzy zapytaniami, Symfony2
przechowuje atrybuty w ciasteczku używając natywnej obsługi sesji w PHP.

W prosty sposób możemy zapisywać jak i odczytywać dane z sesji w kontrolerze::

    $session = $this->getRequest()->getSession();

    // zapis atrybutu do ponownego użycia w późniejszym zapytaniu użytkownika
    $session->set('foo', 'bar');

    // w innym kontrolerze dla innego zapytania
    $foo = $session->get('foo');

    // ustawienie lokalizacji użytkownika
    $session->setLocale('fr');

Możesz także przechowywać małe wiadomości które będą dostępne tylko w najbliższym
zapytaniu::

    // zapis wiadomości dla następnego zapytania (w kontrolerze)
    $session->setFlash('notice', 'Congratulations, your action succeeded!');

    // wyświetlenie wiadomości w kolejnym zapytania (w szablonie)
    {{ app.session.flash('notice') }}

Jest to przydatne gdy chcesz ustawić wiadomość o powodzeniu przed przekierowaniem
użytkownika do innej strony (która pokaże wiadomość).

Zabezpieczone Zasoby
--------------------

Symfony Standard Edition posiada bardzo prostą konfigurację bezpieczeństwa,
która pasuje do większości potrzeb:

.. code-block:: yaml

    # app/config/security.yml
    security:
        encoders:
            Symfony\Component\Security\Core\User\User: plaintext

        role_hierarchy:
            ROLE_ADMIN:       ROLE_USER
            ROLE_SUPER_ADMIN: [ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

        providers:
            in_memory:
                users:
                    user:  { password: userpass, roles: [ 'ROLE_USER' ] }
                    admin: { password: adminpass, roles: [ 'ROLE_ADMIN' ] }

        firewalls:
            dev:
                pattern:  ^/(_(profiler|wdt)|css|images|js)/
                security: false

            login:
                pattern:  ^/demo/secured/login$
                security: false

            secured_area:
                pattern:    ^/demo/secured/
                form_login:
                    check_path: /demo/secured/login_check
                    login_path: /demo/secured/login
                logout:
                    path:   /demo/secured/logout
                    target: /demo/

Ta konfiguracja wymaga aby użytkownicy musieli być zalogowani dla każdego
z URLi zaczynających się od ``/demo/secured/`` oraz definiuje dwóch
użytkowników: ``user`` oraz ``admin``.
Ponadto, użytkownik ``admin`` posiada rolę ``ROLE_ADMIN``, która zawiera rolę
``ROLE_USER`` (zobacz ustawienie ``role_hierarchy``).

.. tip::

    Dla czytelności, w tym przykładzie konfiguracji, hasła są zapisane w
    czystym tekście, ale możesz użyć dowolnego algorytmu mieszania poprzez
    zmienienie sekcji ``encoders``.

Wywołując URL ``http://localhost/Symfony/web/app_dev.php/demo/secured/hello``
użytkownik zostanie automatycznie przekierowany do formularza logowania, ponieważ
ten zasób jest chroniony przez ``firewall``.

Możesz także wymusić aby akcja wymagała określonej roli za pomocą adnotacji
``@Secure`` w kontrolerze::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Route("/hello/admin/{name}", name="_demo_secured_hello_admin")
     * @Secure(roles="ROLE_ADMIN")
     * @Template()
     */
    public function helloAdminAction($name)
    {
        return array('name' => $name);
    }

Teraz, zaloguj się jako ``user`` (który *nie* ma roli ``ROLE_ADMIN``) i
z zabezpieczonej strony hello, kliknij w link "Hello resource secured".
Symfony2 powinno zwrócić 403 HTTP, wskazując że użytkownik ma "zabroniony"
dostęp do tego zasobu.

.. note::

    Warstwa bezpieczeństwa Symfony2 jest bardzo elastyczna i posiada wiele
    różnych dostawców użytkownika (jak jeden dla Doctrine ORM) oraz dostawców
    uwierzytelniania (podstaowe HTTP, HTTP digest, czy certyfikaty X.509).
    Przeczytaj rozdział ":doc:`/book/security`" aby dowiedzieć się więcej jak
    ich używać oraz jak je skonfigurować.

Cache
-----

Jak tylko Twoja strona zacznie generować więcej ruchu, będziesz chciał uniknąć
ciągłego generowania tych samych zasobów. Symfony2 używa nagłówków cache HTTP
do zarządzania zasobami cache. Dla prostych strategi cache, użyj wygodnej
adnotacji ``@Cache()``::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     * @Cache(maxage="86400")
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

W tym przykładzie, zasób będzie trzymany w cache przez jeden dzień. Możesz także
użyć walidacji zamiast wygasania lub też kombinacji tych dwóch jeśli to bardziej
dopasowuje się do Twoich potrzeb.

Cachowanie zasobów jest zarządzane przez wbudowany w Symfony2 reverse proxy.
Ale jako że cache jest zarządzany przez regularne nagłówki cache HTTP, możesz
zamienić wbudowany reverse proxy z Varnish lub też Squid.

.. note::

    Ale co gdy nie możesz cachować całych stron? Symfony2 posiada rozwiązanie
    poprzez Edge Side Includes (ESI), które jest wspierane natywnie.
    Dowiedz się więcej na ten temat z rozdziału ":doc:`/book/http_cache`" książki.

Podsumowanie
------------

Wszystko w temacie, i nie jestem pewien czy spędziliśmy pełnych 10 minut.
Zwięźle omówiliśmy bundle w pierwszej części, i wszystkie funkcje które
poznaliśmy dotychczas są częścią rdzenia "framework bundle".
Dzięki bundlom, wszystko w Symfony2 może być rozszerzone oraz zamienione.
Ale to jest tematem :doc:`kolejnej części kursu<the_architecture>`.
