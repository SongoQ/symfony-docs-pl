.. highlight:: php
   :linenothreshold: 2

.. index::
   single: kontener usług
   single: wstrzykiwanie zależności; kontener

Kontener usług
==============

Nowoczesna aplikacja PHP jest pełna obiektów. Jeden obiekt może ułatwiać dostarczanie
wiadomości e-mail, podczas gdy inny może utrzymywać informacje w bazie danych.
W aplikacji można utworzyć obiekt, który zarządza magazynem produktów lub inny
obiekt przetwarzający dane z API firm trzecich. Chodzi o to, że nowoczesna aplikacja
wykonuje wiele rzeczy i jest zorganizowana z wielu obiektów, które obsługują
poszczególne zadania.

Tematyka tego rozdziału dotyczy specjalnego obiektu PHP w Symfony2, który pomaga
w utworzeniu instancji, zorganizowaniu i pobieraniu wielu obiektów aplikacji.
Obiekt ten, nazywany **kontenerem usług**, umożliwia ujednolicenie i scentralizowanie
sposobu, w jaki są konstruowane obiekty w aplikacji. Kontener istotnie ułatwia życie
projektantowi, jest bardzo szybki i wzmacnia architekturę promującą wielokrotne
wykorzystanie i rozdzielenie kodu. Ponieważ wszystkie rdzenne klasy Symfony2
wykorzystują kontener, to dowiesz się jak rozszerzać, konfigurować i używać każdy
obiekt w Symfony2. W dużej mierze kontener usług jest największym czynnikiem dużej
prędkości i elastyczności Symfony2.

Konfiguracja i używanie kontenera usług jest łatwe. Pod koniec tego rozdziału nabędziesz
umiejętności swobodnego tworzenia własnych obiektów poprzez kontener i dostosowywania
obiektów z dowolnego pakietu osób trzecich. Będziesz w stanie pisać kod, który jest
bardziej dostosowany do wielokrotnego użytku, sprawdzalny i właściwie podzelony,
po prostu dlatego, że kontener usług sprawia pisanie kodu tak łatwym.

.. index::
   single: kontener usług; Co to jest usługa?

Co to jest usługa?
------------------

Mówiąc prosto, :term:`usługa` jest obiektem PHP, który wykonuje jakieś
"globalne" zadanie. Usługa jest używana w aplikacji gdy zachodzi potrzeba zapewnienia
konkretnej funkcjonalności (np. dostarczania wiadomości e-mail). Aby uzyskać usługę
w swojej aplikacji wystarczy po prostu napisać klasę PHP z jakimś kodem, który pozwoli
osiągać zaplanowane zadanie.

.. note::

    Co do zasady, obiekt PHP jest usługą jeśli jest stosowany globalnie w aplikacji.
    Pojedyncza usługa ``Mailer`` jest używana globalnie do wysyłania wiadomości e-mail,
    podczas gdy wiele obiektów ``Message``, które dostarczają wiadomości  e-mail nie
    są usługą. Podobnie obiekt ``Product`` nie jest usługa, ale obiekt utrzymujący
    obiekty ``Product`` w bazie danych jest usługą.

Co jest w tym wielkiego? Zaletą myślenia w kategoriach "usług" jest to, że zaczniesz
myśleć o oddzielnych porcjach funkcjonalności dla szeregu usług w swojej aplikacji.
Ponieważ każda usługa ma tylko jedno zadanie, to łatwo uzyskać dostęp do każdej
usługi i zastosować jej funkcjonalność, gdy jest taka potrzebna. Poszczególna usługa
może być łatwiej testowana i konfigurowana, ponieważ jest oddzielona od innych
funkcjonalności w aplikacji. Taka koncepcja nazywana jest
`architekturą zorientowana na usługi`_ i nie jest charakterystyczna tylko dla Symfony2
czy innych projektów PHP. Tworzenie struktury aplikacji wokół zestawu niezależnych
klas usług jest dobrze znane i zalecane w ramach najlepszych praktyk programowania
obiektowego. Posiadanie tych umiejętności jest kluczem do bycia dobrym programistą
w prawie każdym języku programowania.

.. index::
   single: kontener usług; co to jest kontener usług?

Co to jest kontener usług?
--------------------------

:term:`Kontener usług<kontener usług>` (lub *kontener wstrzykiwania zależności*)
jest prostym obiektem PHP, który zarządza konkretyzacją usług (np. obiektami).

Na przykład załóżmy, że mamy prosta klasę PHP, która dostarcza wiadomości e-mail.
Bez kontenera usług, trzeba ręcznie utworzyć obiekt, gdy jest to potrzebne::

    use Acme\HelloBundle\Mailer;

    $mailer = new Mailer('sendmail');
    $mailer->send('ryan@foobar.net', ...);

Jest to dość łatwe. Przykładowa klasa ``Mailer`` umożliwia skonfigurowanie metody
używanej do dostarczania wiadomości e-mail (np. ``sendmail``, ``smtp`` itd.). 
Lecz co jeśli chce się korzystać z usługi pocztowej w innym miejscu? Na pewno nie
chce się powtarzać konfiguracji poczty w każdym miejscu, w którym używa się obiektu
``Mailer``. Co jeśli zachodzi potrzeba zmiany wartości parametru ``transport`` z
``sendmail`` na ``smtp`` w całej aplikacji? Trzeba wówczas dotrzeć do konfiguracji
każdego miejsca utworzenia usługi ``Mailer`` i zmienić.

.. index::
   single: kontener usług; konfigurowanie usług

Tworzenie i konfigurowanie usług w kontenerze
---------------------------------------------

Lepszym rozwiązaniem jest spowodowanie, aby kontener usług sam utworzył obiekt
``Mailer``. W tym celu musi się "nauczyć" kontener jak tworzyć usługę ``Mailer``.
Wykonuje się to w pliku konfiguracyjnym, który może mieć format YAML, XML lub PHP:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        services:
            my_mailer:
                class:        Acme\HelloBundle\Mailer
                arguments:    [sendmail]

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <services>
            <service id="my_mailer" class="Acme\HelloBundle\Mailer">
                <argument>sendmail</argument>
            </service>
        </services>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('my_mailer', new Definition(
            'Acme\HelloBundle\Mailer',
            array('sendmail')
        ));

.. note::

    Podczas rozruchu Symfony2 buduje kontener usług wykorzystując konfigurację
    aplikacji (domyślnie ``app/config/config.yml``). Egzekwowany plik jest
    wskazywany przez metodę ``AppKernel::registerContainerConfiguration()``,
    która ładuje plik konfiguracyjny stosowany dla danego środowiska (np.
    ``config_dev.yml`` dla środowiska ``dev`` lub ``config_prod.yml`` dla ``prod``).

Instancja obiektu ``Acme\HelloBundle\Mailer`` jest teraz dostępna przez kontener
usług. Kontener jest dostępny w każdym zwykłym kontrolerem Symfony2, w którym
można uzyskać dostęp do usług kontenera poprzez skrótową metodę ``get()``::

    class HelloController extends Controller
    {
        // ...

        public function sendEmailAction()
        {
            // ...
            $mailer = $this->get('my_mailer');
            $mailer->send('ryan@foobar.net', ...);
        }
    }

Kiedy odpytuje się usługę ``my_mailer`` z kontenera, kontener konstruuje obiekt
i go zwraca. To kolejna ważna zaleta kontenera usług. Mianowicie, usługa nie jest
nigdy wykonywana, dopóki nie jest potrzebna. Jeśli zdefiniowało się usługę lecz
nigdy nie użyto jej w żądaniu, to usługa ta nie nigdy nie będzie stworzona.
Oszczędza to pamięć i zwiększa szybkość aplikacji. Oznacza to również, że
definiowaniu wielu usług pozostaje właściwie bez wpływu na wydajność.
Usługi które nie są używane, nie są konstruowane.

Usługa ``Mailer`` jest tworzona tylko raz i ta sama instancja jest zwracana za
każdą prośbą o usługę, co stanowi dodatkowa zaletę. Jest to prawie zawsze zachowanie,
jakie się potrzebuje (jest większa elastyczność i możliwości), ale jak to jest
wyjaśnione w artykule ":doc:`/cookbook/service_container/scopes`", można skonfigurować
usługę, która ma wiele instancji.

.. _book-service-container-parameters:

Parametry usługi
----------------

Tworzenie nowych usług (np. obiektów) poprzez kontener jest bardzo proste.
Parametry czynią zdefiniowane usługi lepiej zorganizowanymi i bardziej elastycznymi:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        "%my_mailer.class%"
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
            <parameter key="my_mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                <argument>%my_mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

Końcowy wynik jest taki sam jak poprzednio - różnicą jest tylko to, jak zdefiniowano
tą usługę. Przez otocznie łańcuchów ``my_mailer.class`` i ``my_mailer.transport``
znakiem procenta (``%``), kontener wie, aby szukać parametrów z tymi nazwami.
Gdy budowany jest kontener, to wyszukiwana jest wartość każdego parametru i zostają
one użyte w definicji usługi.

.. note::

    Znak procentu wewnątrz parametru lub argumentu, jako część łańcucha, musi zostać
    zabezpieczona drugim znakiem procentu:

    .. code-block:: xml

        <argument type="string">http://symfony.com/?foo=%%s&bar=%%d</argument>

Celem parametrów jest podawanie informacji do usług. Oczywiście nie ma niczego
złego w definiowaniu usługi bez stosowania parametrów. Parametry mają jednak wiele
zalet:

* rozdziela i oraganizuje wszystkie "opcje" usługi w ramach jednego klucza
  ``parameters``;

* wartości parametrów mogą być używane w definicjach różnych usług;

* gdy tworzy się usługę w pakiecie (omówiono to będzie wkrótce) używanie parametrów
  sprawia, że usługa może być łatwiej dostosowywana w aplikacji.

Wybór używania czy nie używania parametrów należy do programisty. Pakiety wysokiej
jakości zawsze używają parametrów aby uczynić usługę przechowywaną  w kontenerze
bardziej konfigurowalną. Natomiast dla usług definiowanych w aplikacji może nie być
potrzebna elastyczność parametrów.

Parametry tablicowe
~~~~~~~~~~~~~~~~~~~

Parametry nie muszą być tylko zwykłymi łańcuchami – mogą być również tablicami.
W formacie XML, dla wszystkich parametrów tablicowych trzeba użyć atrybutu
type="collection".

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        parameters:
            my_mailer.gateways:
                - mail1
                - mail2
                - mail3
            my_multilang.language_fallback:
                en:
                    - en
                    - fr
                fr:
                    - fr
                    - en

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="my_mailer.gateways" type="collection">
                <parameter>mail1</parameter>
                <parameter>mail2</parameter>
                <parameter>mail3</parameter>
            </parameter>
            <parameter key="my_multilang.language_fallback" type="collection">
                <parameter key="en" type="collection">
                    <parameter>en</parameter>
                    <parameter>fr</parameter>
                </parameter>
                <parameter key="fr" type="collection">
                    <parameter>fr</parameter>
                    <parameter>en</parameter>
                </parameter>
            </parameter>
        </parameters>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.gateways', array('mail1', 'mail2', 'mail3'));
        $container->setParameter('my_multilang.language_fallback', array(
            'en' => array('en', 'fr'),
            'fr' => array('fr', 'en'),
        ));


Importowanie innych zasobów konfiguracji kontenera
--------------------------------------------------

.. tip::

    W tym rozdziale pliki konfiguracyjne usług są określane jako *zasoby*. Ma to
    na celu podkreślenie faktu, że choć większość zasobów konfiguracyjnych jest
    plikami (np. YAML, XML, PHP), to Symfony2 jest tak elastyczny, że można załadować
    zasoby konfiguracyjne z dowolnego miejsca (np. bazy danych lub nawet poprzez
    zewnętrzny serwis internetowy).

Kontener usług jest budowany przy użyciu pojedynczego zasobu konfiguracyjnego
(domyślnie ``app/config/config.yml``). Wszystkie inne konfiguracje usług
(włączając w to konfigurację rdzenną Symfony2 i konfigurację jakiegoś pakietu
osób trzecich) musi być zaimportowana z wewnątrz tego pliku taki lub inny sposób.
Daje to absolutną elastyczność usługom w aplikacji.

Zewnętrzna konfiguracja usługi może być zaimportowana na dwa różne sposoby.
Pierwsza i najbardziej popularna metoda, to wykorzystanie dyrektywy ``imports``.
Druga metoda, będąca elastyczną i preferowaną metodą importu konfiguracji usług
z pakietów osób trzecich, jest wytłumaczona
:ref:`dalej<service-container-extension-configuration>`.

.. index::
   single: kontener usług; *imports*

.. _service-container-imports-directive:

Importowanie konfiguracji z wykorzystaniem dyrektywy ``imports``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dotychczas umieszczaliśmy definicję kontenera usługi ``my_mailer`` bezpośrednio
w pliku konfiguracyjnym aplikacji (np. ``app/config/config.yml``). Oczywiście,
ponieważ sama klasa ``Mailer`` umieszczona jest wewnątrz pakietu ``AcmeHelloBundle``,
to wydaje się być bardziej sensownym umieszczenie definicji kontenera  ``my_mailer``
wewnątrz pakietu.

Najpierw przeniesiemy definicję kontenera ``my_mailer`` do nowego pliku zasobu
kontenera wewnątrz ``AcmeHelloBundle``. Jeśli jeszcze nie istnieją katalogi
``Resources`` lub ``Resources/config``, to trzeba je utworzyć.

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        "%my_mailer.class%"
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
            <parameter key="my_mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                <argument>%my_mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php
       :linenos:

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

Definicja sama w sobie się nie zmieniła, jedynie jej lokalizacja. Oczywiście kontener
usługi nie wie nic o nowym pliku zasobu. Na szczęście, można łatwo zaimportować plik
zasobu używając klucza ``imports`` w konfiguracji aplikacji.

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        imports:
            - { resource: @AcmeHelloBundle/Resources/config/services.yml }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <imports>
            <import resource="@AcmeHelloBundle/Resources/config/services.xml"/>
        </imports>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $this->import('@AcmeHelloBundle/Resources/config/services.php');

Dyrektywa ``imports`` umożliwia aplikacji dołączenie zasobu konfiguracyjnego kontenera
usług z dowolnej lokalizacji (najczęściej z pakietów). Lokalizacja ``resource`` dla
plików to ścieżka bezwzględna do pliku zasobu. Specjalna składnia ``@AcmeHello``
rozwiązuje ścieżkę pakietu ``AcmeHelloBundle``. Pozwala to określić ścieżkę do zasobu
bez potrzeby jej zmiany, gdy  przeniesie się później ``AcmeHelloBundle`` do innego
katalogu.

.. index::
   single: kontener usług; rozszerzenie konfiguracji

.. _service-container-extension-configuration:

Importowanie konfiguracji poprzez rozszerzenia konfiguracji
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Podczas programowania w Symfony2, najczęściej wykorzystuje się dyrektywę ``imports``
do importu konfiguracji kontenera z pakietów już utworzonych specjalnie dla swojej
aplikacji. Konfiguracja kontenera osób trzecich, w tym rdzennych usług Symfony2
realizowana jest zazwyczaj inną metodą, która jest bardziej elastyczna i łatwiejsza
w skonfigurowaniu w aplikacji.

Oto jak to działa. Wewnętrznie, każdy pakiet definiuje swoje usługi przeważnie tak,
jak widzieliśmy to do tej pory. Mianowicie, pakiet używa jeden lub więcej plików
konfiguracyjnych (zwykle XML) do określenia parametrów i usług dla tego pakietu.
Jednak zamiast importować każdy z tych zasobów bezpośrednio z konfiguracji swojej
aplikacji używając dyrektywę ``imports``, można wywołać **rozszerzenie kontenera usług**
wewnątrz pakietu, który się wykorzystuje. Rozszerzenie kontenera usług jest klasą PHP
utworzona przez autora pakietu, aby osiągnąć dwie rzeczy:

* zaimportowania  wszystkich zasobów kontenera usług potrzebnych do konfiguracji
  usług dla pakietu;

* dostarczenie semantycznej, prostej konfiguracji, tak aby pakiet mógł być konfigurowany
  bez interakcji ze zwykłymi parametrami konfiguracji kontenera usług pakietu.

Innymi słowami, rozszerzenie kontenera usług konfiguruje usługi dla pakietu w imieniu
programisty, który go wykorzystuje. Jak zobaczymy za moment, rozszerzenie takie
dostarcza sensownego interfejsu wysokiego poziomu dla konfiguracji pakietu.

Weźmy dla przykładu ``FrameworkBundle``, rdzenny pakiet frameworka Symfony2. Obecność
niżej podanego kodu w konfiguracji aplikacji wywoła rozszerzenie kontenera usług
wewnątrz ``FrameworkBundle``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            secret:          xxxxxxxxxx
            form:            true
            csrf_protection: true
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config secret="xxxxxxxxxx">
            <framework:form />
            <framework:csrf-protection />
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <!-- ... -->
        </framework>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'secret'          => 'xxxxxxxxxx',
            'form'            => array(),
            'csrf-protection' => array(),
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),

            // ...
        ));

Podczas przetwarzania konfiguracji, kontener wyszukuje rozszerzenia, które może
obsłużyć dyrektywę konfiguracyjną ``framework``. Jest wywoływane rozszerzenie z
zapytania umieszczonego w ``FrameworkBundle`` oraz ładowana jest konfiguracja
usługi dla ``FrameworkBundle``. Jeśli usunie się całkowicie klucz ``framework``
z pliku konfiguracyjnego aplikacji, usługi rdzenia Symfony2 nie zostaną załadowane.
Chodzi o to, że ma się kontrolę: framework Symfony2 nie zawiera żadnej magii ani
nie przetwarza żadnych akcji, nad którą programista nie ma kontroli.

Oczywiście można zrobić o wiele więcej niż tylko "aktywować" rozszerzenie kontenera
``FrameworkBundle``. Każde rozszerzenie pozwala łatwo dostosować pakiet bez martwienia
się o to jak są zdefiniowane usługi wewnętrzne.

W tym przypadku rozszerzenie pozwala dostosować konfigurację ``error_handler``,
``csrf_protection``, ``router``  i wiele więcej. Wewnętrznie ``FrameworkBundle``
używa opcji określonych tutaj do definiowania i konfigurowania usług specyficznych
dla tego pakietu. Pakiet zajmuje się tworzeniem wszystkiego, co jest niezbędna dla
dyrektyw ``parameters`` i ``services`` w kontenerze usług, umożliwiając równocześnie
aby wiele konfiguracji mogło być łatwo dostosowanych. Dodatkową korzyścią jest to,
że większość rozszerzeń kontenera usług jest również wystarczająco inteligentna aby
sprawdzać poprawność, informując o brakujących opcjach, lub o niewłaściwych typach
danych.

Podczas instalowania lub konfigurowania pakietu, trzeba zapoznać się z dokumentacją
pakietu w zakresie dostarczanych w nim usług i jak te usługi powinny być instalowane
i konfigurowane. Opcje dostępne dla rdzennych pakietów Symfony2 są opisane
w :doc:`Reference Guide</reference/index>`.

.. note::

   Kontener usług natywnie rozpoznaje tylko dyrektywy ``parameters``, ``services``
   i ``imports``. Wszystkie inne dyrektywy są obsługiwane przez rozszerzenie kontenera
   usług.

Jeśli chce się udostępnić w swoich pakietach przyjazną dla użytkowników konfigurację,
to warto przeczytać ":doc:`/cookbook/bundles/extension`".

.. index::
   single: kontener usług; usługi referencyjne (wstrzykiwane)

Usługi referencyjne (wstrzykiwane)
----------------------------------

Jak dotychczas oryginalna usługa ``my_mailer`` była prosta. Pobierała tylko jeden
argument w swoim konstruktorze, który był łatwo konfigurowalny. Jak zaraz zobaczymy,
prawdziwe moc kontenera ujawnia się, gdy trzeba stworzyć usługę, która zależy od jednej
lub wielu innych usług w kontenerze.

Dla przykładu załóżmy, że mamy nową usługę ``NewsletterManager``, która pomaga
zarządzać przygotowaniem i dostarczaniem wiadomości e-mail dla kolekcji adresów.
Oczywiście usługa ``my_mailer`` jest już naprawdę dobra w wysyłaniu wiadomości
e-mail, więc zastosujemy ją wewnątrz ``NewsletterManager`` w celu obsługi faktycznie
dostarczanych wiadomości. Ta przykładowa klasa może wyglądać tak::

    // src/Acme/HelloBundle/Newsletter/NewsletterManager.php
    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Bez użycia kontenera usług, można utworzyć dość łatwo nowy obiekt ``NewsletterManager``
wewnątrz kontrolera::

    use Acme\HelloBundle\Newsletter\NewsletterManager;

    // ...

    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new NewsletterManager($mailer);
        // ...
    }

Podejście to nie jest złe, ale co jeśli później zajdzie potrzeba wyposażenia klasy
``NewsletterManager`` w drugi lub trzeci argument kontrolera? Co jeśli zdecyduje
się o refaktoryzowaniu kodu i zmianie nazwy klasy? W obu przypadkach trzeba znaleźć
każde miejsce, gdzie jest utworzona instancja ``NewsletterManager`` i zmodyfikować
ją. Kontener usług, jak można się tego spodziewać, umożliwia o wiele bardziej atrakcyjne
rozwiązanie:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: [@my_mailer]

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ...>
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer"/>
            </service>
        </services>

    .. code-block:: php
       :linenos:

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer'))
        ));

W formacie YAML, specjalna składnia ``@my_mailer`` informuje kontener, aby
szukał usługi o nazwie ``my_mailer`` i przekazał ten obiekt do konstruktora klasy
``NewsletterManager``. W tym przypadku jednak określona usługa ``my_mailer`` musi
istnieć. Jeśli nie, to zrzucony zostanie wyjątek. Można oznaczyć zależność jako
opcjonalną – zostanie to omówione w następnym rozdziale.

Korzystanie z referencji jest bardzo mocnym narzedziem, które umożliwia
tworzenie niezależnych klas usług z dobrze zdefiniowanymi zależnościami. W tym
przykładzie usługa ``newsletter_manager`` potrzebuje usługi ``my_mailer`` w celu
funkcjonowania. Po określeniu tej zależności w kontenerze usług, kontener zajmie
się całym działaniem instancji obiektów.

Zależności opcjonalne - wstrzykiwanie setera
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Wstrzykiwanie w ten sposób zależności do konstruktora  jest doskonałym sposobem
zapewnienia, aby można byłu używać zależności. Jeśli w klasie występują zależności
opcjonalne, to lepszym rozwiązaniem może być "wstrzykiwanie setera" . **Seterem**
(**ang. setter*) nazywamy tu tzw. akcesor ustawiający (metodę publiczną, która
ustawia w obiekcie prywatne lub chronione właściwości - jej nazwa rozpoczyna się od *set*).
Oznacza to wstrzykiwanie przy użyciu wywołania metody a nie przez konstruktor.
Klasa może wyglądać następująco::

    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Wstrzykiwanie zależności przez metodę setera wymaga zmiany składni:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                calls:
                    - [ setMailer, [ @my_mailer ] ]

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ...>
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php
       :linenos:

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer'),
        ));

.. note::

    Opisane tutaj podejście nazywane jest "wstrzykiwaniem konstruktora" i "wstrzykiwaniem
    setera". Kontener usług Symfony2 również obsługuje "wstrzykiwanie zależności".

Czynienie referencji opcjonalnymi
---------------------------------

Czasami jedna z usług może mieć zależności opcjonalne, co oznacza, że zależność
nie jest wymagana dla prawidłowego działania usługi. W powyższym przykładzie usługa
``my_mailer`` musi istnieć, w przeciwnym razie zostanie zrzucony wyjątek. Przez
zmodyfikowanie definicji usługi ``newsletter_manager`` można uczynić tą referencję
opcjonalną. Kontener wstrzyknie ją, jeśli istnieje, jeśli nie, to nic się nie stanie:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...

        services:
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: [@?my_mailer]

    .. code-block:: xml
       :linenos:

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->

        <services>
            <service id="my_mailer" ...>
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer" on-invalid="ignore" />
            </service>
        </services>

    .. code-block:: php
       :linenos:

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        // ...
        $container->setParameter(
            'newsletter_manager.class',
            'Acme\HelloBundle\Newsletter\NewsletterManager'
        );

        $container->setDefinition('my_mailer', ...);
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference(
                    'my_mailer',
                    ContainerInterface::IGNORE_ON_INVALID_REFERENCE
                )
            )
        ));

W formacie YAML, specjalna składnia ``@?`` informuje kontener usług, że zależność
jest opcjonalna. Oczywiście ``NewsletterManager`` musi również być również napisane
z opcjonalną zależnością::

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Rdzenne usługi Symfony i usługi osób trzecich
---------------------------------------------

Ponieważ rdzenne pakiety Symfony2 jak też wszystkie pakiety osób trzecich konfigurują
i pobierają swoje usługi poprzez kontener, to można łatwo uzyskać do nich dostęp
a nawet użyć je w swoich usługach. Aby zachować prostotę, Symfony2 domyślnie nie
wymaga aby kontrolery były definiowane jako usługi. Ponadto Symfony2 wstrzykuje
cały kontener usług do kontrolera. Na przykład, aby obsłużyć przechowywanie informacji
w sesji użytkownika, Symfony2 dostarcza usługę ``session``, w której można uzyskać
dostęp wewnątrz standardowego kontrolera w następujący sposób::

    public function indexAction($bar)
    {
        $session = $this->get('session');
        $session->set('foo', $bar);

        // ...
    }

W Symfony2 istnieje możliwość stałego korzystania z usług dostarczonych przez rdzeń
Symfony lub inne pakiety osób trzecich do wykonywania zadań takich jak renderowanie
szablonów (``templating``), wysyłanie wiadomości e-mail (``mailer``) lub uzyskiwanie
dostępu do informacji w żądaniu (``request``).

Można pójść o krok dalej, korzystając z tych usług wewnątrz innych usług, które
zostały utworzone dla aplikacji. Zacznijmy od modyfikacji ``NewsletterManager``
w celu wykorzystania rzeczywistej usługi Symfony2 ``mailer`` (zamiast pozornej
usługi ``my_mailer``). Przekażemy również do ``NewsletterManager`` usługę silnika
szablonowania, tak aby można było generować treść wiadomości e-mail poprzez szablon::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Templating\EngineInterface;

    class NewsletterManager
    {
        protected $mailer;

        protected $templating;

        public function __construct(\Swift_Mailer $mailer, EngineInterface $templating)
        {
            $this->mailer = $mailer;
            $this->templating = $templating;
        }

        // ...
    }

Konfiguracja kontenera usług jest łatwa:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            newsletter_manager:
                class:     "%newsletter_manager.class%"
                arguments: [@mailer, @templating]

    .. code-block:: xml
       :linenos:

        <service id="newsletter_manager" class="%newsletter_manager.class%">
            <argument type="service" id="mailer"/>
            <argument type="service" id="templating"/>
        </service>

    .. code-block:: php
       :linenos:

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference('mailer'),
                new Reference('templating'),
            )
        ));

Usługa ``newsletter_manager`` ma teraz dostęp do usług rdzennych ``mailer``
i ``templating``. Jest to powszechny sposób tworzenia usług specyficznych dla
aplikacji, które wykorzystują możliwości różnych usług ze szkieletu.

.. tip::

    Należy się upewnić, że w konfiguracji aplikacji występuje wpis ``swiftmailer``.
    Jak wspomniano w rozdziale :ref:`service-container-extension-configuration`,
    klucz ``swiftmailer`` wywołuje rozszerzenie kontenera usług z ``SwiftmailerBundle``,
    które rejestruje usługę ``mailer``.


.. index::
   single: kontener usług; tagi

.. _book-service-container-tags:

Tagi
----

Usługi skonfigurowane w kontenerze mogą być również oflagowane, w podobny sposób
jak wpisy na blogu. W kontenerze usług tag wskazuje, że dana usługa może być
użyta w określonym celu. Weźmy następujący przykład:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            foo.twig.extension:
                class: Acme\HelloBundle\Extension\FooExtension
                tags:
                    -  { name: twig.extension }

    .. code-block:: xml
       :linenos:

        <service id="foo.twig.extension" class="Acme\HelloBundle\Extension\FooExtension">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php
       :linenos:

        $definition = new Definition('Acme\HelloBundle\Extension\FooExtension');
        $definition->addTag('twig.extension');
        $container->setDefinition('foo.twig.extension', $definition);

Tag ``twig.extension`` jest specjalną etykietą, którą wykorzystuje ``TwigBundle``
podczas konfiguracji. Przez przyporządkowanie usłudze taga ``twig.extension``
pakiet wie, że usługa ``foo.twig.extension`` powinna zostać zarejestrowana jako
rozszerzenie Twiga w Twigu. Innymi słowami, Twig wyszukuje wszystkie usługi z tagiem
``twig.extension`` i automatycznie rejestruje je jako rozszerzenia.

Tagi są więc sposobem na poinformowanie Symfony2 lub pakietów osób trzecich,
że usługa powinina zostać zarejestrowana lub użyta w jakiś specjalny sposób przez
pakiet.

Poniżej znajduje się lista tagów dostępnych w pakietach rdzennych Symfony2.
Każda z nich ma inny wpływ na usługę a wiele tagów wymaga dodatkowych argumentów
(nie licząc parametru ``name``).

Listę wszystkich dostępnych tagów w rdzennym pakiecie Symfony Framework można
znaleźć w dokumencie :doc:`/reference/dic_tags`.

.. index::
   single: kontener usług; informacje o dostępnych usługach

Uzyskiwanie informacji o dostępnych usługach
--------------------------------------------

Można dowiedzieć się jakie usługi zostały zarejestrowane w kontenerze, używając
konsoli. Aby wyświetlić wszystkie usługi i klasy dla poszczególnych trzeba uruchomić
polecenie:

.. code-block:: bash

    $ php app/console container:debug

Domyślnie pokazywane są tylko publiczne usługi, ale można też wyświetlić usługi
prywatne:

.. code-block:: bash

    $ php app/console container:debug --show-private

Można uzyskać bardziej szczegółowe informacje na temat danej usługi podając jej
identyfikator:

.. code-block:: bash

    $ php app/console container:debug my_mailer

Czytaj więcej
-------------

* :doc:`/components/dependency_injection/compilation`
* :doc:`/components/dependency_injection/definitions`
* :doc:`/components/dependency_injection/factories`
* :doc:`/components/dependency_injection/parentservices`
* :doc:`/components/dependency_injection/tags`
* :doc:`/cookbook/controller/service`
* :doc:`/cookbook/service_container/scopes`
* :doc:`/cookbook/service_container/compiler_passes`
* :doc:`/components/dependency_injection/advanced`

.. _`architekturą zorientowana na usługi`: http://pl.wikipedia.org/wiki/Architektura_zorientowana_na_us%C5%82ugi