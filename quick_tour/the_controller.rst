.. highlight:: php
   :linenothreshold: 2

Kontroler
=========

Wciąż z nami po pierwszych dwóch częściach? Zaczynasz się uzależniać od Symfony2!
Bez zbędnych ceregieli, zobacz co kontrolery potrafią zrobić dla Ciebie.

Formaty Używania
----------------

Obecnie, aplikacje internetowe powinna dostarczać coś więcej niż tylko
strony HTML. Od XML dla kanałów RSS lub usług internetowych, do JSON dla żądań Ajax,
istnieje wiele różnych formatów do wyboru. Obsługa tych formatów w Symfony2 jest prosta.
Popraw wydajność definicji tras przez dodanie domyślnej wartości xml dla zmiennej
``_format``::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route("/hello/{name}", defaults={"_format"="xml"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Dzięki wykorzystaniu formatu zapytania (określonego jako wartość ``_format``),
Symfony2 automatycznie wybierze odpowiedni szablon, w tym przypadku ``hello.xml.twig``:

.. code-block:: xml+php

    <!-- src/Acme/DemoBundle/Resources/views/Demo/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

To wszystko co jest niezbędne. Dla standardowych formatów Symfony2 będzie również
automatycznie wybierać najlepszy nagłówek ``Content-Type`` dla odpowiedzi. Jeżeli
chce się obsługiwać różne formaty dla pojedyńczej akcji, to trzeba zamiat tego
użyć w ścieżce trasy wieloznacznika {_format}::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    // ...

    /**
     * @Route("/hello/{name}.{_format}", defaults={"_format"="html"}, requirements={"_format"="html|xml|json"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Kontroler będzie teraz również wywoływany dla ścieżek URL takich jak ``/demo/hello/Fabien.xml``
lub ``/demo/hello/Fabien.json``.

Wpis ``requirements`` określa wyrażenie regularne, jakie wieloznacznik musi dopasować.
W tym przykładzie, jeżeli zażąda się zasobu ``/demo/hello/Fabien.js``, to otrzyma się
komunikat błędu 404 HTTP, jako że podany łańcuch nie pasuje do wymagania ``_format``.

Przekierowanie i przekazanie
----------------------------

Jeśli chce się przekierować użytkownika do innej strony, to trzeba użyć metody
``redirect()``::

    return $this->redirect($this->generateUrl('_demo_hello', array('name' => 'Lucas')));

Metoda ``generateUrl()`` jest identyczna z funkcją ``path()`` którą użyliśmy w szablonach.
Jako argumenty, przyjmuje nazwę trasy oraz tablicę parametrów i zwraca związany
przyjazny adres URL.

Możesz także w łatwy sposób przekazać akcję do innej akcji używając metodę ``forward()``.
Wewnętrznie Symfony2 wykonuje "pod-żądanie" oraz zwraca z niego obiekt ``Response``::

    $response = $this->forward('AcmeDemoBundle:Hello:fancy', array('name' => $name, 'color' => 'green'));

    // zrób coś z obiektem Response lub też zwróć go bezpośrednio


Pobieranie informacji z zapytania
---------------------------------

Poza wartościami wieloznaczników tras kontroler ma również dostęp do obiektu
``Request``::

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

Utrzymywanie danych w sesji
---------------------------

Pomimo że protokół HTTP jest bezstanowy, Symfony2 dostarcza przyjemny obiekt sesji,
który reprezentuje klienta (może to być realna osoba używająca przeglądarki, bot
lub usługa internetowa). Symfony2 przechowuje w pliku cookie atrybuty sesji pomiędzy
dwoma żądaniami, wykorzystując natywną sesję PHP.


Przechowywania i pobierania informacji z sesji można łatwo uzyskać w dowolnym kontrolerze::

    $session = $this->getRequest()->getSession();

    // przechowanie atrybutu do ponownego użycia w późniejszym żądaniu użytkownika
    $session->set('foo', 'bar');

    // w innym kontrolerze dla innego żądania
    $foo = $session->get('foo');

    // użycie domyślnej wartości, jeśli klucz nie istnieje
    $filters = $session->set('filters', array());

Można także przechowywać małe komunikaty (zwane komunikatami fleszowymi) które będą
dostępne w kolejnych żądaniach::

    // przechowanie (w kontrolerze) komunikatu dla następnych żądań
    $session->getFlashBag()->add('notice', 'Congratulations, your action succeeded!');

    // wyświetlenie z powrotem komunikatu w następnym żądaniu (w szablonie)

    {% for flashMessage in app.session.flashbag.get('notice') %}
        <div>{{ flashMessage }}</div>
    {% endfor %}

Jest to przydatne gdy chce się ustawić komunikat o powodzeniu przed przekierowaniem
użytkownika do innej strony (która wyświetli ten komunikat). Należy pamiętać, że
przy stosowaniu funkcji has() zamiast get(), komunikat fleszowy nie będzie usunięty
i pozostanie dostępny dla następnych żądań.

Bezpieczeństwo zasobów
----------------------

Symfony Standard Edition dostarczany jest z prostą konfiguracją bezpieczeństwa,
która wystarcza dla potrzeb większości powszechnych zastosowań:

.. code-block:: yaml
   :linenos:

    # app/config/security.yml
    security:
        encoders:
            Symfony\Component\Security\Core\User\User: plaintext

        role_hierarchy:
            ROLE_ADMIN:       ROLE_USER
            ROLE_SUPER_ADMIN: [ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

        providers:
            in_memory:
                memory:
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

Taka konfiguracja wymaga od użytkowników zalogowania się dla dowolnego adresu URL
zaczynającego się od ``/demo/secured/`` i określa dwóch właściwych użytkowników:
``user`` i ``admin``. Ponadto użytkownik ``admin`` ma rolę ``ROLE_ADMIN``, która
obejmuje też rolę ``ROLE_USER`` (zobacz na ustawienie ``role_hierarchy``).


.. tip::

    W tym przykładzie, dla czytelności konfiguracji, hasła są zapisane w zwyłym
    tekście, ale można użyć dowolnego algorytmu mieszajacego poprzez zmienienie
    sekcji ``encoders``.

Przechodząc do adresu ``http://localhost/Symfony/web/app_dev.php/demo/secured/hello``,
użytkownik automatycznie zostanie przekierowany do formularza logowania, gdyż zasób
jest chroniony przez firewall.

.. note::

    Warstwa bezpieczeństwa Symfony2 jest bardzo użyteczna i dostarczana jest z wieloma
    "dostawcami" użytkowników (czyli mechanizmami umożliwiającymi dostęp do danych
    użytkownika z różnych źródeł, takimi jak Doctrine ORM) i "dostawcami" uwierzytelniania
    (takimi jak HTTP Basic, HTTP Digest  lub certyfikaty X509). Przeczytaj rozdział
    ":doc:`/book/security`" podręcznika w celu uzyskania więcej informacji.

Buforowanie zasobów
-------------------

Gdy tylko witryna zacznie generować więcej ruchu, zachodzi potrzeba uniknnięcia
ciągłego generowania tych samych zasobów. Symfony2 używa nagłówków buforowania
HTTP do zarządzania zasobami pamięci podręcznej. Dla prostych strategi buforowania,
mozna użyć wygodnej adnotacji ``@Cache()``::

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

W tym przykładzie, zasób będzie buforowany przez jeden dzień. Można także
użyć walidacji zamiast wygasania lub kombinacji tych dwóch technik, jeśli to lepsze
rozwiązanie dla konkretnej sytuacji.

Buforowanie zasobów jest zarządzane przez wbudowane w Symfony2 odwrotny serwer pośredniczący
(*ang. reverse proxy*). Ponieważ buforowanie jest zarządzane z wykorzystaniem zwykłych
nagłówków buforowania HTTP, to można zamienić wbudowane odwrotny serwer pośredniczący
na Varnish lub Squid i łatwo skalować swoją aplikację.


.. note::

    Lecz co, jeżeli nie można buforować całej strony? Symfony2 rozwiązuje ten problem
    przez natywnie wspierane Edge Side Includes (ESI). Zapoznaj się ze szczegółami
    na stronie ":doc:`/book/http_cache`". 

Podsumowanie
------------

To wszystko w tym temacie i nie jestem pewny, czy czytanie tego zajęło Ci pełne 10 minut.
W pierwszej części pokrótce zapoznaliśmy się z pakietami poznając, że wszystkie dotychczas
poznane funkcjonalności są składnikiem pakietu rdzenia frameworka i wiemy już też, że
dzięki pakietom wszystko w Symfony2 może zostać rozszerzone lub wymienione. To właśnie
jest tematem :doc:`następnej części przewodnika<the_architecture>`.
