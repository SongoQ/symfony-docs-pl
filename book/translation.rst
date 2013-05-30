.. highlight:: php
   :linenothreshold: 2

.. index::
   tłumaczenia, umiedzynarodowienie
   single: tłumaczenia; ustawienia regionalne
   single: tłumaczenia; identyfikator regionalny

Tłumaczenia
===========

Pojęcie **umiędzynarodowienie** (*ang. internationalization*) (często skracane do `i18n`_)
odnosi się do procesu uabstrakcyjnienia łańcuchów tekstowych i innych specyficznych
dla języka elementów aplikacji, tworzących warstwę aplikacji, w której te abstrakcyjne
wyrażenia mogą być tłumaczone i przekształcane na wyrażenia specyficzne dla **ustawień
regionalnych** aplikacji (tj. języka i kraju). W przypadku tekstu oznacza to opakowanie
tekstu w funkcję zdolną do translacji tekstu (lub "komunikatu") na język użytkownika.::

    // ten tekst zawsze będzie drukowany w języku angielskim
    echo 'Hello World';

    // ten tekst może zostać przetłumaczony na język użytkownika końcowego lub
    // domyślnie na język angielski
    echo $translator->trans('Hello World');

.. note::

    Pojęcie **ustawienia regionalne** (*ang. locale*) dotyczy mniej więcej języka
    i kraju użytkownika. Oznaczane jest przez jakiś łańcuch tekstowy, który aplikacja
    używa do zarządzania tłumaczeniami i innymi różnicami formatów (np. formatem walutowym).
    Zalecanym oznaczeniem ustawień regionalnch jest format złożony z kodu językowego
    wg. `ISO639-1`_, znaku podkreślenia (``_``) i następnie kodu kraju
    wg. `ISO3166 Alpha-2`_ (np. ``pl_PL`` dla polski/Polska). Oznaczenie ustawień
    regionalnych aplikacji będziemy w tym podręczniku nazywać **identyfikatorem
    regionalnym**.
    
W tym rozdziale dowiesz się, jak przygotować aplikację do obsługi wielu ustawień
regionalnych, a następnie, jak stworzyć tłumaczenia. Ogólnie rzecz biorąc, proces
ten ma kilka wspólnych kroków:

#. Udostępnienie i skonfigurowanie komponentu ``Translation`` Symfony;

#. Uabstrakcyjnienie łańcuchów tekstowych (czyli "komunikatów") przez opakowanie
   ich w wywołania ``Translator``;

#. Stworzenie zasobów translacyjnych dla każdego obsługiwanego ustawienia regionalnego,
   dla którego tłumaczony jest każdy komunikat w aplikacji;

#. Ustalenie, ustawienie i zarządzanie ustawieniem regionalnym użytkownika dla
   żądań i opcjonalnie w całej sesji użytkownika.

.. index::
   single: tłumaczenia; konfiguracja

.. _configuration:

Konfiguracja
------------

Tłumaczenia są obsługiwane przez :term:`usługę<usługa>` ``Translator``, która
wykorzystuje ustawienie regionalne użytkownika do wyszukania i zwrócenia
przetłumaczonych komunikatów. Przed stosowanie tego trzeba udostępnić ``Translator``
w konfiguracji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            translator: { fallback: en }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:translator fallback="en" />
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'translator' => array('fallback' => 'en'),
        ));

Opcja ``fallback`` określa podstawowy identyfikator regionalny, który zostanie wybrany,
gdy nie istnieją tłumaczenia dla ustawień regionalnych użytkownika.

.. tip::

    Gdy nie istnieje tłumaczenie dla ustawienia regionalnego użytkownika, translator
    najpierw będzie próbował znaleźć tłumaczenie dla określonego języka (przykładowo
    ``pl` jeśli identyfikatorem regionalnym jest ``pl_PL``). Jeśli tego nie znajdzie,
    to wyszuka tłumaczenie dla podstawowego identyfikatora regionalnego (podanego
    w opcji ``fallback``).

Identyfikator regionalny używany w tłumaczeniach jest również przechowywany w żądaniu.
Jest to zwykle ustawiane poprzez atrybut ``_locale`` w konfiguracji trasy
(zobacza :ref:`book-translation-locale-url`).

.. index::
   single: tłumaczenia; podstawowe tłumaczenie

Podstawowe tłumaczenie
----------------------

Tłumaczenie tekstu jest realizowane przez usługę ``translator``
(:class:`Symfony\\Component\\Translation\\Translator`). Aby przetłumaczyć blok tekstu (nazywany tu *komunikatem*), trzeba użyć metody
:method:`Symfony\\Component\\Translation\\Translator::trans`.
Załóżmy na przykład, że tłumaczymy prosty komunikat wewnątrz kontrolera::

    // ...
    use Symfony\Component\HttpFoundation\Response;

    public function indexAction()
    {
        $translated = $this->get('translator')->trans('Symfony2 is great');

        return new Response($translated);
    }

Gdy wykonywany jest ten kod, to Symfony2 będzie próbowało przetłumaczyć komunikat
"Symfony2 is great" w oparciu o wartość opcji ``locale`` użytkownika.
Aby to działało, trzeba powiadomić Symfony2 jak ma przetłumaczyć komunikat, wykorzytując
"zasób translacyjny", który jest kolekcją tłumaczeń komunikatów dla danego identyfikatora
regionalnego. Ten "słownik" może być stworzony w różnych formatach, XLIFF jest
formatem zalecanym:

.. configuration-block::

    .. code-block:: xml
       :linenos:

        <!-- messages.pl.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>Symfony2 jest wielkie</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php
       :linenos:

        // messages.pl.php
        return array(
            'Symfony2 is great' => 'Symfony2 jest wielkie',
        );

    .. code-block:: yaml
       :linenos:

        # messages.pl.yml
        Symfony2 is great: Symfony2 jest wielkie

Teraz, gdy językiem ustawienia regionalnego użytkownika jest język polski, to komunikat
zostanie przetłumaczony jako ``Symfony2 jest wielkie``.

.. index::
   single: tłumaczenia; zasoby translacyjne

Proces tłumaczenia
~~~~~~~~~~~~~~~~~~

W celu właściwego przetłumaczenia komunikatu, Symfony2 stosuje prosty proces:

* Zostaje określona wartość ``locale`` dla bieżącego użytkownika, która jest zawarta
  w żądaniu (lub przechowywana jako ``_locale`` w sesji);

* Z zasobów translacyjnych dla określonej wartości ``locale`` (np. ``pl_PL``)
  ładowany jest katalog przetłumaczonych komunikatów . Ładowane są również komunikaty
  dla podstawowego identyfikatora regionalnego (określonego w opcji ``fallback``)
  i dodawane są one do katalogu jeśli jeszcze w nim nie istnieją. Końcowym rezultatem
  jest wielki "słownik" tłumaczeń w postaci katalogu komunikatów (*ang. message
  cataogue*). Szczegóły omówione są w rozdziale :ref:`message-catalogues`.

* Jeśli komunikat znajduje się w katalogu, to zwracane jest tłumaczenie. Jeśli nie,
  to zwracany jest oryginalny komunikat.

Gdy używa się metody ``trans()``, Symfony2 wyszukuje dokładny łańcuch tekstowy w
odpowiednim katalogu komunikatów i go zwraca (jeśli istnieje).

.. index::
   single: tłumaczenia; komunikaty zastępcze
   single: tłumaczenia; zasoby translacyjne

Komunikaty zastępcze
~~~~~~~~~~~~~~~~~~~~

Czasem komunikat zwiera zmienną, która musi być tłumaczona::

    // ...
    use Symfony\Component\HttpFoundation\Response;

    public function indexAction($name)
    {
        $translated = $this->get('translator')->trans('Hello '.$name);

        return new Response($translated);
    }

Jednak utworzenie tłumaczenia tego łańcucha nie jest możliwe, gdyż translator
będzie próbował wyszukać komunikat łącznie z wartościa tekstową zmiennej
(np. "Hello Ryan" lub "Hello Fabien"). Zamiast pisać tłumaczenia dla każdej
możliwej iteracji zmiennej ``$name``, można zamienić zmienną "wieloznacznikiem"::

    // ...
    use Symfony\Component\HttpFoundation\Response;

    public function indexAction($name)
    {
        $translated = $this->get('translator')->trans(
            'Hello %name%',
            array('%name%' => $name)
        );

        return new Response($translated);
    }

Symfony2 będzie teraz wyszukiwać tłumaczenie dla surowego komunikatu (``Hello %name%``)
i następnie zamieni wieloznacznik ``%name%`` wartością zmiennej ``$name``.
Tworzenie tłumaczenia realizowane jest tak jak wcześniej:

.. configuration-block::

    .. code-block:: xml
       :linenos:

        <!-- messages.pl.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Hello %name%</source>
                        <target>Witaj %name%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php
       :linenos:

        // messages.pl.php
        return array(
            'Hello %name%' => 'Witaj %name%',
        );

    .. code-block:: yaml
       :linenos:

        # messages.pl.yml
        'Hello %name%': Witaj %name%

.. note::

    Wieloznaczniki mogą mieć dowolna formę, jako że rekonstruowany jest pełny
    komunikat przy użyciu `funkcji strtr`_ PHP. Jednak podczas tłumaczenia
    szablonów Twig wymagana jest notacja ``%var%`` i jest to powszechna,
    sensowna konwencja , godna naśladowania.

Jak widać, tworzenie tłumaczenia, to proces dwuetapowy:

#. Uabstrakcyjnienie komunikatu, który musi zostać przetłumaczony przez
   przetworzenie go w translatorze ``Translator``.

#. Stworzenie tłumaczenia dla komunikatu dla każdego identyfikatora regionalnego,
   który został wybrany do zastosowania w aplikacji.

Drugi etap polega na utworzeniu katalogów komunikatów określających tłumaczenia
dla dowolnej liczby różnych identyfikatorów regionalnych.

.. index::
   single: tłumaczenia; katalogi komunikatów
   single: tłumaczenia; zasoby translacyjne

.. _message-cataloques:

Katalogi komunikatów
--------------------

Podczas tłumaczenia komunikatu, Symfony2 kompiluje katalog komunikatów (*ang.
message catalogue*) dla ustawienia regionalnego użytkownika i wyszukuje właściwe
tłumaczenia komunikatów. Katalog komunikatów jest podobny do katalogu z tłumaczeniami
ale jest specyficzny dla identyfikatora regionalnego. Na przykład, katalog dla identyfikatora
regionalnego ``pl_PL`` może zawierać następujące tłumaczenie:

.. code-block:: text

    Symfony2 is Great => Symfony2 jest wielkie

Utworzenie plików tłumaczeń leży w gestii programisty (lub tłumacza) aplikacji
i18n. Tłumaczenia są przechowywane w systemie plików i są dostępne dla Symfony
dzięki pewnej konwencji.

.. tip::

    Za każdym razem, gdy stworzy się nowy zasób translacyjny (lub zainstaluje pakiet,
    który zawiera zasób translacyjny), trzeba wyczyścić pamięć podręczną, tak aby
    Symfony mogło zauważyć nowy zasób translacyjny:
    

    .. code-block:: bash

        $ php app/console cache:clear

.. index::
   single: tłumaczenia; zasoby translacyjne
   single: tłumaczenia; konwencja nazewnicza

Lokalizacja tłumaczeń i konwencja nazewnicza
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 wyszukuje pliki komunikatów (czyli tłumaczenia) w następujących lokalizacja:

* katalog ``<kernel root directory>/Resources/translations``;

* katalog ``<kernel root directory>/Resources/<bundle name>/translations``;

* katalog pakietu ``Resources/translations/``.

Wymienione lokalizacje ułożone są tutaj wg priorytetu pierwszeństwa. Można więc
zastąpić komunikaty translacyjne pakietu umieszczając swoje pliki w dwóch wyżej
wymienionych katalogów.

Mechanizmy przesłaniania działają na poziomie kluczy: w pliku komunikatów o najwyższym
priorytecie musi się wymienić tylko przesłaniane klucze. Gdy klucz nie zostaje
znaleziony w pliku komunikatu, translator automatycznie przechodzi do plików komunikatów
niższego poziomu.

Ważna jest też nazwa plików tłumaczeń jako że, w Symfony2 stosowana jest konwencja
ustalania szczegółowej informacji o tłumaczeniach. Każdy plik komunikatu musi być
nazwany zgodnie z następującym wzorcem: ``domain.locale.loader``:

* **domain**: opcjonalny sposób organizowania komunikatów w grupach (np. ``admin``,
  ``navigation`` lub domyślnie``messages``) - zobacz `Using Message Domains`_;

* **locale**: identyfikator regionalny tłumaczenia (np. ``en_GB``, ``en``, itd.);

* **loader**: wskazuje jak Symfony2 powinien załadować i parsować plik (tj. ``xliff``,
  ``php`` lub ``yml``).

Część ``loader`` może być nazwą dowolnego zarejestrowanego loadera. Domyślnie
Symfony obsługuje::

* ``xliff``: plik XLIFF;
* ``php``:   plik PHP;
* ``yml``:  plik YAML.

Wybór loadera zależy tylko do Ciebie i jest to tylko kwestia gustu.

.. note::

    Tłumaczenia można również przechowywać w bazie danych lub w jakiś inny sposób,
    dostarczając własną klasę implementującą interfejs
    :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`.

.. index::
   single: tłumaczenia; zasoby translacyjne

Tworzenie tłumaczeń
~~~~~~~~~~~~~~~~~~~

Akt tworzenia plików tłumaczeń jest ważną częścią tzw. **lokalizacji**
(*ang. localization*) (często skracanej jako `l10n`_). Pliki tłumaczeń zawierają
serie par *identyfikator-tłumaczenie* dla danej domeny i identyfikatora regionalnego.
Źródłem jest identyfikator poszczególnego tłumaczenia i może to być komunikat
w języku głównego ustawienia regionalnego aplikacji (np. "Symfony is great") lub
unikalny identyfikator (np. "symfony2.great" – patrz na ramkę poniżej):

.. configuration-block::

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/DemoBundle/Resources/translations/messages.pl.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>Symfony2 is great</source>
                        <target>Symfony2 jest wielkie</target>
                    </trans-unit>
                    <trans-unit id="2">
                        <source>symfony2.great</source>
                        <target>Symfony2 jest wielkie</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // src/Acme/DemoBundle/Resources/translations/messages.pl.php
        return array(
            'Symfony2 is great' => 'Symfony2 jest wielkie',
            'symfony2.great'    => 'Symfony2 jest wielkie',
        );

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/translations/messages.pl.yml
        Symfony2 is great: Symfony2 jest wielkie
        symfony2.great:    Symfony2 jest wielkie

Symfony2 znajdzie te pliki i użyje je  podczas tłumaczenia komunikatu "Symfony2
is great" lub "symfony2.great" w języku polskimi (czyli ``pl_PL``).

.. sidebar:: Używanie komunikatów rzeczywistych lub opartych na słowach kluczowych

    Przykład ten ilustruje dwie różne filozofie podczas tworzenia komunikatów,
    które mają być przetłumaczone::

        $translated = $translator->trans('Symfony2 is great');

        $translated = $translator->trans('symfony2.great');

    W pierwszej metodzie komunikaty są pisane w języku domyślnego ustawienia
    regionalnego (w tym przypadku angielski). Treść komunikatu jest stosowana
    jako "id" podczas tworzenia tłumaczeń.

    W drugiej metodzie komunikaty są rzeczywistymi "słowami kluczowymi", które
    przekazują ideę komunikatu. Komunikat ze słowem kluczowym jest następnie
    stosowany jako "identyfikator" jakichkolwiek tłumaczeń. W tym przypadku
    tłumaczenia muszą zostać wykonane dla domyślnego ustawienia regionalnego
    (np. do tłumaczenia ``symfony2.great`` na ``Symfony2 is great``).

    Druga metoda jest przydatna, ponieważ klucze komunikatów nie muszą być zmieniane
    w każdym pliku tłumaczeń, jeśli zdecydujesz aby komunikat musiał właściwie
    odczytywać "Symfony2 is really great" w domyślny ustawieniu regionalnym.

    Wybór którejś z tych metod zależy od Ciebie, ale częściej zalecany jest format
    ze słowami kluczowymi.

    Dodatkowo formaty plików ``php`` i ``yaml`` obsługują zagnieżdżanie identyfikatorów,
    co pozwala uniknąć powtarzania się, jeśli stosuje się dla identyfikatorów  słowa
    kluczowe anie rzeczywisty tekst:
    
    .. configuration-block::

        .. code-block:: yaml
           :linenos:

            symfony2:
                is:
                    great: Symfony2 is great
                    amazing: Symfony2 is amazing
                has:
                    bundles: Symfony2 has bundles
            user:
                login: Login

        .. code-block:: php
           :linenos:

            return array(
                'symfony2' => array(
                    'is' => array(
                        'great'   => 'Symfony2 is great',
                        'amazing' => 'Symfony2 is amazing',
                    ),
                    'has' => array(
                        'bundles' => 'Symfony2 has bundles',
                    ),
                ),
                'user' => array(
                    'login' => 'Login',
                ),
            );

    Wiele poziomów jest spłaszczanych do pojedynczej pary *identyfikator-tłumaczenie*
    przez dodanie znaku kropki (.) pomiedzy każdym poziomem, dlatego powyższy przykład
    jest równoważny z tym:

    .. configuration-block::

        .. code-block:: yaml
           :linenos:

            symfony2.is.great: Symfony2 is great
            symfony2.is.amazing: Symfony2 is amazing
            symfony2.has.bundles: Symfony2 has bundles
            user.login: Login

        .. code-block:: php
           :linenos:

            return array(
                'symfony2.is.great' => 'Symfony2 is great',
                'symfony2.is.amazing' => 'Symfony2 is amazing',
                'symfony2.has.bundles' => 'Symfony2 has bundles',
                'user.login' => 'Login',
            );

.. index::
   single: tłumaczenia; domeny komunikatów

Używanie domen komunikatów
--------------------------

Jak to widzieliśmy, pliki komunikatów są organizowane w różne ustawienia regionalne,
dla których są tłumaczone. Pliki komunikatów mogą również zostać dalej organizowane
w "domeny". Podczas tworzenia plików komunikatów, domena jest pierwszą częścią
nadawanej nazwy pliku. Domyślna domena, to ``messages``. Załóżmy na przykład że,
w celach organizacyjnych tłumaczenia zostały podzielone na trzy różne domeny:
``messages``, ``admin`` i ``navigation``. Polskie tłumaczenia miałyby następujące
pliki komunikatów:

* ``messages.pl.xliff``
* ``admin.pl.xliff``
* ``navigation.pl.xliff``

Podczas tłumaczenia łańcuchów, które nie znajdują się w domyślnej domenie
(``messages``), trzeba określić domenę jako trzeci argument metody ``trans()``::

    $this->get('translator')->trans('Symfony2 is great', array(), 'admin');

Symfony2 będzie teraz wyszukiwał komunikat w domenie ``admin`` ustawienia regionalnego
użytkownika.

.. index::
   single: tłumaczenia; ustawienie regionale

Obsługa ustawienia regionalengo użytkownika
-------------------------------------------

Ustawienie regionalne bieżącego użytkownika jest zapisywane w żądaniu i jest dostępne
poprze obiekt ``request``::

    // access the request object in a standard controller
    $request = $this->getRequest();

    $locale = $request->getLocale();

    $request->setLocale('en_US');

.. index::
   single: tłumaczenia; ustawienie regionalne rezerwowe
   single: tłumaczenia; ustawienie regionalne domyślne

Możliwe jest też zapisywanie ustawienia regionalnego w sesji zamiast ustalania
go na podstawie każdego żądania. Jeśli się to zrobi, to każde kolejne żądanie
będzie miało już to ustawienie.

.. code-block:: php

    $this->get('session')->set('_locale', 'en_US');

Przeczytaj rozdział :ref:`book-translation-locale-url` traktujący o ustawieniu
identyfikatora regionalnego poprzez trasę.

Rezerwowe i domyślne ustawienie regionalne
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli identyfikator regionalny nie został ustawiony jawnie w sesji,
to ``Translator`` użyje parametru konfiguracyjnego ``fallback_locale``.
Domyślna wartość tego parametru, to ``en`` (zobacz :ref:`configuration`).

Ewentualnie można zagwarantować aby identyfikator regionalny był ustawiany domyślnie
na każde żądanie użytkownika przez określenie ``default_locale`` dla frameworka:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            default_locale: en

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:default-locale>en</framework:default-locale>
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'default_locale' => 'en',
        ));

.. versionadded:: 2.1
     Parametr ``default_locale`` jest pierwotnie określony w kluczu sesji,
     jednak od wersji 2.1 został on przeniesiony. Jest tak dlatego, że identyfikator
     regionalny jest teraz ustawiany w żądaniu a nie w sesji.

.. _book-translation-locale-url:

Ustawienie regionalne a adres URL
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ponieważ można przechowywać identyfikator regionalny w sesji, to wydaje się
być kuszące, aby używać tego samego adresu URL do wyświetlania zasobu w różnych
językach w oparciu o ustawienie lokalne użytkownika. Na przykład, strona
``http://www.example.com/contact`` może być wyświetlana w języku angielskim dla
jednego użytkownika a po polsku dla innego. Niestety narusza to podstawową zasadę
internetu, że określony adres URL zwraca ten sam zasób niezależnie od użytkownika.
Następnym problemem jest jest kwestia, którą wersję strony mają indeksować
wyszukiwarki.

Lepszą zasadą jest dołączenie identyfikatora regionalnego do adresu URL. Takie
rozwiązanie jest w pełni obsługiwane przez system trasowania, przy użyciu specjalnego
parametru ``_locale``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        contact:
            path:      /{_locale}/contact
            defaults:  { _controller: AcmeDemoBundle:Contact:index, _locale: en }
            requirements:
                _locale: en|pl|de

    .. code-block:: xml
       :linenos:

        <route id="contact" path="/{_locale}/contact">
            <default key="_controller">AcmeDemoBundle:Contact:index</default>
            <default key="_locale">en</default>
            <requirement key="_locale">en|pl|de</requirement>
        </route>

    .. code-block:: php
       :linenos:

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/{_locale}/contact', array(
            '_controller' => 'AcmeDemoBundle:Contact:index',
            '_locale'     => 'en',
        ), array(
            '_locale'     => 'en|pl|de',
        )));

        return $collection;

Gdy używa się specjalnego parametru ``_locale`` w definicji trasy, dopasowana trasa
będzie automatycznie ustawiana na sesję użytkownika. Innymi słowami, jeśli użytkownik
odwiedza adres URI ``/pl/contact``, identyfikator regionalny ``pl`` zostanie automatycznie
ustawiony jako identyfikator ustawienia regionalnego sesji użytkownika.

Teraz można już korzystać z ustawień regionalnych użytkownika do tworzenia tras
dla innych tłumaczonych stron w aplikacji.

.. index::
   single: tłumaczenia; liczba mnoga
   single: tłumaczenia; pluralizacja komunikatu

Tworzenie liczby mnogiej
------------------------

Stosowanie liczby mnogiej w komunikatach jest trudnym zagadnieniem, jako że zasady
tworzenia liczby mnogiej są różne w różnych językach i mogą być bardzo złożone.
Na przykład, oto matematyczna reprezentacja zasad tworzenia liczby mnogiej dla
języka rosyjskiego i polskiego::

    (($number % 10 == 1) && ($number % 100 != 11))
        ? 0
        : ((($number % 10 >= 2)
            && ($number % 10 <= 4)
            && (($number % 100 < 10)
            || ($number % 100 >= 20)))
                ? 1
                : 2
    );

Jak widać, w języku rosyjskim i polskim, ma się trzy różne formy liczby mnogiej,
której tu przypisano kolejno indeks 0, 1 lub 2. Każda z tych form jest inna
i dlatego tłumaczenie dla każdej z tych form jest również inne.

Gdy tłumaczenie ma różne formy ze względu ma liczbę mnogą, to można zapewnić
wszystkie formy w postaci łańcucha rozdzielanego znakiem kreski pionowej (``|``)::

    'There is one apple|There are %count% apples'

Taką czynność będziemy tu nazywać **pluralizowaniem komunikatu** a komunikaty dla
których trzba wykonać lub wykonało się taką czynność - **komunikatami pluralizowanymi**.  

Aby przetłumaczyć komunikaty pluralizowane trzeba użyć metody 
:method:`Symfony\\Component\\Translation\\Translator::transChoice`::

    $translated = $this->get('translator')->transChoice(
        'There is one apple|There are %count% apples',
        10,
        array('%count%' => 10)
    );

Drugi argument (``10`` w tym przykładzie), jest liczbą opisywanych obiektów
i jest użyty dla określenia które tłumaczenie zostało tu użyte a także do wypełnienia
wieloznacznika ``%count%``.

Na podstawie podanej liczby translator wybiera prawidłową formę liczby mnogiej.

W języku angielskim większość słów ma formę liczby pojedynczej dla pojedynczego
obiektu i jedną formę liczby mnogiej gdy ilość obiektów jest inna niż jeden
(0, 2, 3...). Tak więc, jeśli ``count`` wynosi ``1``, to translator użyje do
tłumaczenia pierwszego łańcucha (``There is one apple``). W innym przypadku
użyje ``There are %count% apples``.

Oto francuskie tłumaczenie::

    'Il y a %count% pomme|Il y a %count% pommes'

Chociaż łańcuchy te wyglądają podobnie (składają się z dwóch pod-łańcuchów
oddzielonych znakiem kreski pionowej), to zasady języka francuskiego są inne –
pierwsza forma (nie mnoga) zostaje użyta, gdy ilość ``count`` wynosi ``0`` lub
``1``. Tak więc translator automatycznie użyje pierwszego łańcucha 
(``Il y a %count% pomme``) gdy ``count`` wynosi ``0`` or ``1``.

Każdy język ma swoje własne zasady tworzenia liczby mnogiej, niektóre języki mają
nawet sześć różnych form liczby mnogiej ze skomplikowanymi zasadami ich tworzenia.
Zasady te są proste dla języka angielskiego, czy francuskiego, ale dla języka polskiego
czy rosyjskiego, to tłumacz może chciałby uzyskać jakąś wskazówkę, jaką zasadę użyć
w którym łańcuchu. Aby pomóc tłumaczom można opcjonalnie użyć "etykiety” dla każdego
znacznika::

    'one: There is one apple|some: There are %count% apples'

    'none_or_one: Il y a %count% pomme|some: Il y a %count% pommes'
    
    'n1: To jest jedno jabłko|n2-n4: To są %count% jabłka|some: To jest %count% jabłek

Etykiety te są tu tylko wskazówką dla tłumacza i nie wpływają na logikę użytą do
określenia tego, jaką formę liczby mnogiej się używa. Etykiety mogą być dowolnym
opisami tekstowymi i kończą się dwukropkiem (``:``). Etykiety nie muszą być takie
same w oryginalnym komunikacie i tłumaczeniu – mogą się różnić, lub mogą w ogóle
nie występować.

.. tip::

    Translator nie używa etykiet. Bierze tylko pod uwagę pozycję występowania
    pod-łańcucha w łańcuchu. Dla polskiego i rosyjskiego tłumaczenia należy
    więc uwzględniać indeksy określone w algorytmie przedstawionym na początku
    rozdziału.

.. index::
   single: tłumaczenia; technika interwałowa

Pluralizowanie z zastosowaniem techniki interwałowej
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Najłatwiejszym sposobem pluralizowania komunikatów jest wymuszenia na Symfony2
aby użyło wewnętrznej logiki do wyboru właściwego łańcucha, na podstawie kolejności
występowania tego łańcucha. Czasami jednak potrzeba większej kontroli nad tłumaczeniem
lub zachodzi szczególny przypadek w tłumaczeniu liczby mnogiej (na przykład dla ``0``
lub gdy liczba jest ujemna). W takich przypadkach można użyć **techniki interwałowej**,
opartej na interwałach, które są szczególnym typem matematycznych przedziałów.
**Interwał** jest centralnym pojeciem `arytmetyki interwałów`_::

    '{0} There are no apples|{1} There is one apple|]1,19] There are %count% apples|[20,Inf] There are many apples'

W technice interwałowej wykorzystuje się notację `ISO 31-11`_. Powyższy ciąg określa
cztery różne przedziały: dokładnie ``0``, dokładnie ``1``, ``2-19`` oraz ``20`` i wyżej.

Można również mieszać zasady techniki interwałowej ze zasadami standardowymi.
W takim przypadku, jeśli liczba nie jest dopasowywana przez określony interwał,
to mają zastosowanie standardowe zasady po usunięciu zasad techniki interwałowej::

    '{0} There are no apples|[20,Inf] There are many apples|There is one apple|a_few: There are %count% apples'

Na przykład, dla ``1`` jabłka, zostanie użyta standardowa zasada ``There is one apple``.
Dla ``2-19`` jabłek, wybrana będzie druga standardowa zasada ``There are %count% apples``.

Klasa :class:`Symfony\\Component\\Translation\\Interval` może reprezentować skończony zbiór liczb::

    {1,2,3,4}

lub liczby zawarte pomiędzy dwoma innymi liczbami::

    [1, +Inf[
    ]-1,2[

Lewym ogranicznikiem może być ``[`` (włącznie) lub ``]`` (wyłącznie).
Prawym ogranicznikiem może być ``[`` (wyłącznie) lub ``]`` (włącznie).
Oprócz liczb można używać  ``-Inf`` i ``+Inf`` dla nieskończoności.

.. index::
   single: tłumaczenia; szablony
   single: szablonowanie; tłumaczenia

Tłumaczenia w szablonach
------------------------

Tłumaczenia występują przede wszystkim w szablonach. Symfony2 dostarcza
własnego wsparcia zarówno dla szablonów Twiga jak i PHP.

.. _book-translation-tags:

Szablony Twiga
~~~~~~~~~~~~~~

Symfony2 dostarcza wyspecjalizowanych znaczników Twiga (``trans`` i ``transchoice``)
do pomocy w tłumaczeniu komunikatów statycznych bloków tekstu:

.. code-block:: jinja
   :linenos:

    {% trans %}Hello %name%{% endtrans %}

    {% transchoice count %}
        {0} There are no apples|{1} There is one apple|]1,Inf] There are %count% apples
    {% endtranschoice %}

Znacznik ``transchoice`` automatycznie pobiera zmienną ``%count%`` z bieżącego
kontekstu i przekazuje ją do translatora. Mechanizm ten działa tylko wtedy, gdy
używa się wieloznacznika w formie ``%var%``.

.. tip::

    Jeśli zachodzi potrzeba użycia w tekście znaku procenta (``%``), to należy
    go zabezpieczyć, podwajając ten znak: ``{% trans %}Procent: %percent%%%{% endtrans %}``

Można również określić domenę komunikatu i przekazać dodatkowe zmienne:

.. code-block:: jinja
   :linenos:

    {% trans with {'%name%': 'Fabien'} from "app" %}Hello %name%{% endtrans %}

    {% trans with {'%name%': 'Fabien'} from "app" into "fr" %}Hello %name%{% endtrans %}

    {% transchoice count with {'%name%': 'Fabien'} from "app" %}
        {0} %name%, there are no apples|{1} %name%, there is one apple|]1,Inf] %name%, there are %count% apples 
    {% endtranschoice %}

.. _book-translation-filters:

Filtry ``trans`` i ``transchoice`` mogą być zastosowane do tłumaczenia *tekstów
zmiennych* i wyrażeń złożonych:

.. code-block:: jinja
   :linenos:

    {{ message|trans }}

    {{ message|transchoice(5) }}

    {{ message|trans({'%name%': 'Fabien'}, "app") }}

    {{ message|transchoice(5, {'%name%': 'Fabien'}, 'app') }}

.. tip::

    Przy użyciu znaczników translacyjnych osiąga się ten sam efekt co z użyciem
    filtrami, ale jest jedna subtelna różnica: automatyczne zabezpieczenie zmiennych
    znakami ucieczki (*ang. output escaping*) jest osiągalne tylko przy użyciu filtrów.
    Innymi słowami, jeśli chce się mieć pewność, że zmienne w tłumaczeniu nie są
    zabezpieczone znakami ucieczki, to  należy zastosować filtr ``raw`` po filtrze
    translacji:

    .. code-block:: jinja
       :linenos:

            {# text pomiędzy znacznikami nie wogóle jest zabezpieczony znakami ucieczki #}
            {% trans %}
                <h3>foo</h3>
            {% endtrans %}

            {% set message = '<h3>foo</h3>' %}

            {# Łańcuchy i zmienne tłumaczone poprzez filtr są domyślnie zabezpieczone #}
            {{ message|trans|raw }}
            {{ '<h3>bar</h3>'|trans|raw }}

.. tip::

    Można ustawić domenę tłumaczenia dla całego szablonu Twiga używając pojedynczego
    znacznika:

    .. code-block:: jinja

           {% trans_default_domain "app" %}

    Proszę zwrócić uwagę, ze wpływa to tylko na bieżący szablon, a anie na szablony
    "dołączone" (w celu uniknięcia skutków ubocznych).

.. versionadded:: 2.1
    Znacznik ``trans_default_domain`` jest nowością w Symfony2.1

Szablony PHP
~~~~~~~~~~~~

Usługa tłumaczeń jest dostępna w szablonach PHP za pośrednictwem helpera ``translator``:

.. code-block:: html+php
   :linenos:

    <?php echo $view['translator']->trans('Symfony2 is great') ?>

    <?php echo $view['translator']->transChoice(
        '{0} There is no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10)
    ) ?>

Wymuszanie tłumaczeń regionalnych
---------------------------------

Podczas tłumaczenia komunikatu Symfony2 używa identyfikatora regionalnego z bieżącego
żądania lub z wartości parametru ``fallback`` w razie konieczności. Można również
ręcznie określić identyfikator regionalny do zastosowania w tłumaczeniu::

    $this->get('translator')->trans(
        'Symfony2 is great',
        array(),
        'messages',
        'pl_PL'
    );

    $this->get('translator')->transChoice(
        '{0} There are no apples|{1} There is one apple|]1,Inf[ There are %count% apples',
        10,
        array('%count%' => 10),
        'messages',
        'pl_PL'
    );

Tłumaczenie treści z bazy danych
--------------------------------

Tłumaczenie treści z bazy danych powinno być obsługiwane przez rozszrzenie
`Translatable Extension`_ doctrine. Więcej informacji znajdziesz w dokumentacji
tej biblioteki.

.. _book-translation-constraint-messages:

Tłumaczenie komunikatów ograniczeń
----------------------------------

Najlepszym sposobem na zrozumienie tłumaczeń ograniczeń jest zobaczenie tego w działaniu.
Załóżmy, że stworzyliśmy najzwyklejszy obiekt PHP, który trzeba użyć gdzieś w aplikacji::

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public $name;
    }

Dodajmy ograniczenia do jakichś obsługiwanych metod i ustawmy opcję komunikatu do
tłumaczenia tekstu źródłowego. Na przykład, aby zagwarantować, że właściwość
``$name`` nie jest pusta, dodajmy co następuje:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: { message: "author.name.not_blank" }

    .. code-block:: php-annotations
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank(message = "author.name.not_blank")
             */
            public $name;
        }

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank">
                        <option name="message">author.name.not_blank</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php
       :linenos:

        // src/Acme/BlogBundle/Entity/Author.php

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank(array(
                    'message' => 'author.name.not_blank',
                )));
            }
        }

Stwórzmy plik tłumaczeń w katalogu walidatorów dla komunikatów
ograniczeń, zwykle jest to katalog ``Resources/translations/`` pakietu.
Przeczytaj :ref:`message-cataloques` w celu poznania szczegółów.

.. configuration-block::

    .. code-block:: xml
       :linenos:

        <!-- validators.en.xliff -->
        <?xml version="1.0"?>
        <xliff version="1.2" xmlns="urn:oasis:names:tc:xliff:document:1.2">
            <file source-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="1">
                        <source>author.name.not_blank</source>
                        <target>Please enter an author name.</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php
       :linenos:

        // validators.en.php
        return array(
            'author.name.not_blank' => 'Please enter an author name.',
        );

    .. code-block:: yaml
       :linenos:

        # validators.en.yml
        author.name.not_blank: Please enter an author name.


Podsumowanie
------------

Z komponentem Symfony2 Translation, tworzenie umiędzynarodowionych aplikacji nie
musi być bolesnym procesem i sprowadza się do kilku prostych kroków:

* Uabstrakcyjnienie komunikatów w aplikacji przez owinięcie każdego z nich metodą 
  :method:`Symfony\\Component\\Translation\\Translator::trans` lub
  :method:`Symfony\\Component\\Translation\\Translator::transChoice`;

* Przetłumaczenie każdego komunikatu dla wielu ustawień regionalnych przez utworzenie
  plików tłumaczeń komunikatów. Symfony2 odnajduje i przetwarza każdy plik ponieważ
  jego nazwa zgodna jest z określoną konwencją;

* Zarządzanie ustawieniami regionalnymi, które są przechowywane w żądaniu, ale mogą
  również być ustawione w sesji użytkownika.

.. _`i18n`: http://en.wikipedia.org/wiki/Internationalization_and_localization
.. _`l10n`: http://en.wikipedia.org/wiki/Internationalization_and_localization
.. _`funkcji strtr`: http://www.php.net/manual/en/function.strtr.php
.. _`ISO 31-11`: http://en.wikipedia.org/wiki/Interval_(mathematics)#Notations_for_intervals
.. _`Translatable Extension`: https://github.com/l3pp4rd/DoctrineExtensions
.. _`ISO3166 Alpha-2`: http://en.wikipedia.org/wiki/ISO_3166-1#Current_codes
.. _`ISO639-1`: http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
.. _`arytmetyki interwałów`: https://en.wikipedia.org/wiki/Interval_arithmetic 