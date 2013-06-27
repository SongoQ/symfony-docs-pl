.. index::
   single: konfiguracja; semantyczny
   single: pakiet; konfiguracja rozszerzenia

Jak wyeksponować semantyczną konfigurację pakietu
=================================================

Po otwarciu pliku konfiguracyjnego aplikacji (zazwyczaj ``app/config/config.yml``)
widać wiele różnych konfiguracji "przestrzeni nazw", takie jak ``framework``,
``twig``, i ``doctrine``. Każda z nich konfiguruje określony pakiet, pozwalając
skonfigurować rzeczy na wysokim poziomie, tym samym składając wszystkie
niskopoziomowe, złożone działania na barki pakietu.

Przykładowo następujący przykład instruuje ``FrameworkBundle``, aby ustawił
funkcje integracji formularzy, co pociąga za sobą zdefiniowanie kilku usług,
jak również integracje innych, powiązanych komponentów:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        framework:
            # ...
            form: true

    .. code-block:: xml
       :linenos:

        <framework:config>
            <framework:form />
        </framework:config>

    .. code-block:: php
       :linenos:

        $container->loadFromExtension('framework', array(
            // ...
            'form' => true,
            // ...
        ));

Po utworzeniu pakietu są dwa sposoby na zarządzanie jego konfiguracją:

1. **Normalna konfiguracja usługi** (*łatwe*):

    Można określić swoje usługi w pliku konfiguracyjnym (np. ``services.yml``),
    zlokalizowanym w strukturze plików pakietu, a następnie zaimportować go
    z głównej konfiguracji aplikacji. To jest bardzo proste, szybkie i całkowicie
    skuteczne. Jeśli wykorzystuje się :ref:`parameters<book-service-container-parameters>`,
    wówczas posiada się elastyczność w dostosowywaniu swojego pakietu z konfiguracji
    aplikacji. Aby poznać więcej szczegółów, zobacz ":ref:`service-container-imports-directive`".
    details.

2. **Eksponowanie semantycznej konfiguracji** (*zaawansowane*):

    To jest sposób, w jaki główne pakiety eksponują swoją konfigurację. (jak
    opisano powyżej). Podstawowa idea jest taka, że zamiast pozwalać użytkownikowi
    na nadpisywanie indywidualnych parametrów, pozwala się mu na konfigurację
    tylko kilku z nich, specjalnie przygotowanych wcześniej. Twórca pakietu powinien
    wówczas przeanalizować te opcje, a następnie wczytać usługi wewnątrz klasy
    "Extension". Dzięki ten metodzie, nie będzie trzeba importować żadnych zasobów
    z głównej konfiguracji swojej aplikacji: klasa Extension zajmie się tym wszystkim.

Druga opcja - o której napisano w tym artykule - jest o wiele bardziej elastyczna,
ale wymaga stosunkowo więcej czasu na skonfigurowanie. Jeśli nie ma się pewności,
której metody użyć, dobrym pomysłem jest zacząć od metody #1, by potem w miarę
upływu czasu przejść na metodę #2, o ile oczywiście zajdzie taka potrzeba.

Druga metoda ma kilka szczególnych zalet:

* Dostarcza o wiele więcej, niż tylko proste definiowanie parametrów: konkretna wartość
  opcji może powodować tworzenie wielu definicji usług;

* Daje możliwość hierarchizacji konfiguracji;

* Pozwala na inteligentne łączenie wielu plików konfiguracyjnych (np. ``config_dev.yml``
  i ``config.yml``), które mogłyby nadpisywać swoje własne konfiguracje;

* Umożliwia sprawdzanie poprawności konfiguracji (jeśli używa się :ref:`Configuration Class<cookbook-bundles-extension-config-class>`);

* Pozwala na autouzupełnianie w środowisku IDE podczas tworzenia schematów XSD
dla programistów używających języka XML.

.. sidebar:: Przesłanianie parametrów pakietu

    Jeśli pakiet zawiera klasę Extension, to *nie* powinno się generalnie
    nadpisywać żadnych parametrów kontenera usług z tego pakietu. Ideą jest, że
    jeśli klasa Extension jest obecna w pakiecie, wszelkie konfigurowalne ustawienia
    powinny być obecne w konfiguracji udostępnianej przez tą klasę. Innymi słowy,
    klasa Extension określa wszystkie publicznie obsługiwane ustawienia konfiguracji dla
    których zgodność wstecz będzie utrzymywana.

.. index::
   single: pakiet; rozszerzenie
   single: wstrzykiwanie zależności; rozszerzenia

Tworzenie klasy Extension
-------------------------

Jeśli zdecydowano o wystawieniu semantycznej konfiguracji pakietu, będzie
trzeba najpierw utworzyć nową klasę "Extension", odpowiedzialną za obsługiwanie
tego procesu. Klasa ta powinna mieścić się w katalogu pakietu ``DependencyInjection``,
a jej nazwa powinna zostać utworzona poprzez podmianę końcówki ``Bundle`` z klasy
pakietu na ``Extension``. Przykładowo, klasa Extension pakietu ``AcmeHelloBundle``
miałaby nazwę ``AcmeHelloExtension``::

    // Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\HttpKernel\DependencyInjection\Extension;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeHelloExtension extends Extension
    {
        public function load(array $configs, ContainerBuilder $container)
        {
            // ... gdzie ma miejsce cała cieżka logika
        }

        public function getXsdValidationBasePath()
        {
            return __DIR__.'/../Resources/config/';
        }

        public function getNamespace()
        {
            return 'http://www.example.com/symfony/schema/';
        }
    }

.. note::

    Metody ``getXsdValidationBasePath`` i ``getNamespace`` są wymagane jedynie,
    gdy pakiet zapewnia opcjonalne schematy XSD dla konfiguracji.

Obecność poprzedniej klasy oznacza, że można zdefiniować przestrzeń nazw
konfiguracji ``acme_hello`` w każdym pliku konfiguracyjnym. Przestrzeń ``acme_hello``
tworzona jest z nazwy klasy Extension przez usunięcie słowa ``Extension``,
podmianę na małe litery i użycie podkreśleń w reszcie nazwy. Innymi słowa,
``AcmeHelloExtension`` staje się ``acme_hello``.

Można rozpocząć określanie konfiguracji pod tą przestrzenią nazw natychmiast:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        acme_hello: ~

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

           <acme_hello:config />

           <!-- ... -->
        </container>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array());

.. tip::

    Jeśli stosuje się konwencje nazewnictwa określone powyżej, wówczas metoda
    ``load()`` z klasy Extension jest wywoływana zawsze, oczywiście tak długo, jak
    pakiet jest zarejestrowany w klasie Kernel. Innymi słowy, nawet gdy użytkownik
    nie zapewni żadnej konfiguracji (np. wpis ``acme_hello`` nawet się nie pojawi),
    metoda ``load()`` zostanie wywołana z pustą tablicą ``$configs``. Nadal
    można podać kilka wartości domyślnych dla pakietu, o ile zachodzi taka potrzeba.

Analizowanie tablicy ``$configs``
---------------------------------

Za każdym razem, gdy użytkownik dołącza przestrzeń nazw ``acme_hello`` w pliku
konfiguracyjnym, konfiguracja w nim zawarta jest dodawana do tablicy opcji i
przekazywana do metody ``load()`` w klasie Extension (Symfony2 automatycznie
przekształca XML i YAML do postaci tablicy).

Proszę zapoznać się z następującą konfiguracją:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        acme_hello:
            foo: fooValue
            bar: barValue

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config foo="fooValue">
                <acme_hello:bar>barValue</acme_hello:bar>
            </acme_hello:config>

        </container>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ));

Tablica przekazywana do metody ``load()`` będzie wyglądać tak::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ),
    )

Proszę zauważyć, że jest to *tablica tablic*, a nie tylko prosta, płaska tablica z
wartościami konfiguracji. Jest to zamierzone. Przykładowo, jeśli ``acme_hello``
pojawia się w innym pliku konfiguracyjnym - powiedzmy ``config_dev.yml`` - z
różnymi wartościami pod nim, wówczas finalna tablica mogłaby wyglądać tak::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ),
        array(
            'foo' => 'fooDevValue',
            'baz' => 'newConfigEntry',
        ),
    )

Kolejność dwóch tablic zależy od tego, która z nich została ustawiona jako pierwsza.

Zatem jest to decyzja programisty, w jaki sposób te konfiguracje powinny zostać
połączone ze sobą. Móżna by przykładowo umówić się, że późniejsze wartości nadpiszą
wcześniejsze lub też w jakiś sposób połączą się razem.

Później, w sekcji :ref:`Configuration Class<cookbook-bundles-extension-config-class>`,
wszystko zostanie rozwiązane kompleksowo. Póki co jednak, można połączyć konfiguracje
ręcznie::

    public function load(array $configs, ContainerBuilder $container)
    {
        $config = array();
        foreach ($configs as $subConfig) {
            $config = array_merge($config, $subConfig);
        }

        // ... teraz użyj płaskiej tablicy $config
    }

.. caution::

    Należy upewnić się, czy powyższe techniki łączenia mają sens dla danego pakietu.
    To jest tylko przykład, należy więc uważać, aby nie używać go na oślep.

Używanie metody ``load()``
--------------------------

Zmienna ``$container`` wewnątrz metody ``load()`` odnosi się do kontenera,
który wie tylko o swojej konfiguracji przestrzeni nazw (tzn. nie zawiera informacji
o usługach wczytywanych z innych pakietów). Celem metody ``load()`` jest
manipulacja kontenerem oraz dodawanie i konfigurowanie wszelkich niezbędnych metod lub
usług tego pakietu.

Wczytywanie zasobów zewnętrznej konfiguracji
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jedną, wspólną rzeczą do zrobienia jest wczytanie pliku zewnętrznej konfiguracji,
który może zawierać większość usług używanych w pakiecie. Załóżmy przykładowo,
że plik ``services.xml`` zawiera znaczną część konfiguracji usług w pakiecie::

    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;
    use Symfony\Component\Config\FileLocator;

    public function load(array $configs, ContainerBuilder $container)
    {
        // ... przygotuj zmienną $config

        $loader = new XmlFileLoader(
            $container,
            new FileLocator(__DIR__.'/../Resources/config')
        );
        $loader->load('services.xml');
    }

Można to nawet zrobić warunkowo, bazując na jednej z wartości konfiguracyjnych.
Załóżmy na przykład, że chce się wczytać zestaw usług, o ile przesyłana jest
opcja ``enabled`` i ustawiona na true::

    public function load(array $configs, ContainerBuilder $container)
    {
        // ... prepare your $config variable

        $loader = new XmlFileLoader(
            $container,
            new FileLocator(__DIR__.'/../Resources/config')
        );

        if (isset($config['enabled']) && $config['enabled']) {
            $loader->load('services.xml');
        }
    }

Konfigurowanie usług i ustawianie parametrów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Po załadowaniu kilku ustawień usługi, być może trzeba będzie zmienić konfigurację
w oparciu o niektóre z wartości wejściowych. Załóżmy, że stworzyło się usługę, której
pierwszym argumentem jest jakiś napis "type", którego będzie używała wewnętrznie.
Jeśli chciałoby się uprościć konfigurację tego pakietu użytkownikom wewnątrz
jednego z plików konfiguracyjnych usługi (np. ``services.xml``), powinno się ją zdefiniować
z użyciem pustego parametru - ``acme_hello.my_service_type`` - jako jej pierwszego argumentu:

.. code-block:: xml
   :linenos:

    <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

        <parameters>
            <parameter key="acme_hello.my_service_type" />
        </parameters>

        <services>
            <service id="acme_hello.my_service" class="Acme\HelloBundle\MyService">
                <argument>%acme_hello.my_service_type%</argument>
            </service>
        </services>
    </container>

Dlaczego definiować pusty parametr i przekazywać go do swojej usługi?
Odpowiedzią jest ustawienie tego parametru w klasie Extension, bazującej na
przychodzących wartościach konfiguracyjnych. Założmy na przykład, że chce się
umożliwić użytkownikowi definiowanie opcji *type* pod kluczem o nazwie ``my_type``.
Aby to osiągnąć, należy dodać poniższe linie do metody ``load()``::

    public function load(array $configs, ContainerBuilder $container)
    {
        // ... przygotuj zmienną $config

        $loader = new XmlFileLoader(
            $container,
            new FileLocator(__DIR__.'/../Resources/config')
        );
        $loader->load('services.xml');

        if (!isset($config['my_type'])) {
            throw new \InvalidArgumentException(
                'The "my_type" option must be set'
            );
        }

        $container->setParameter(
            'acme_hello.my_service_type',
            $config['my_type']
        );
    }

Od teraz użytkownik może efektywnie skonfigurować usługę określając wartość
konfiguracji ``my_type``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        acme_hello:
            my_type: foo
            # ...

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config my_type="foo">
                <!-- ... -->
            </acme_hello:config>

        </container>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'my_type' => 'foo',
            ...,
        ));

Parametry globalne
~~~~~~~~~~~~~~~~~~

Podczas konfigurowania kontenera trzeba mieć świadomość o poniższych parametrach
globalnych, które są gotowe do użycia od samego początku:

* ``kernel.name``
* ``kernel.environment``
* ``kernel.debug``
* ``kernel.root_dir``
* ``kernel.cache_dir``
* ``kernel.logs_dir``
* ``kernel.bundles``
* ``kernel.charset``

.. caution::

    Wszystkie nazwy parametrów i usług zaczynające się od ``_`` są zarezerwowane
    przez framework, a nowe nie mogą być definiowane przez pakiety.

.. _cookbook-bundles-extension-config-class:

Walidacja i łączenie z klasą Configuration
------------------------------------------

Do tej pory udało się łączyć tablice konfiguracji ręcznie oraz sprawdzać,
czy wartości konfiguracji są ustawione z użyciem funkcji PHP ``isset()``.
Opcjonalny system *Configuration* jest również dostępny, dzięki któremu
łączenie, walidacja, operowanie na wartościach domyślnych oraz formacie normalizacji
mogą okazać się znacznie prostsze.

.. note::

    Normalizacja formatu odnosi się do faktu, że niektóre formaty - głównie
    XML - powodują powstawanie nieco innych tablic konfiguracyjnych, przez co
    wymagają one "normalizacji", by dopasować się do wszystkiego innego.

Aby skorzystać z tego systemu, można utworzyć klasę ``Configuration``
i zbudować drzewo, które określi konfigurację w tej klasie::

    // src/Acme/HelloBundle/DependencyInjection/Configuration.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('acme_hello');

            $rootNode
                ->children()
                ->scalarNode('my_type')->defaultValue('bar')->end()
                ->end();

            return $treeBuilder;
        }
    }

To jest *bardzo* prosty przykład, pozwala jednak wykorzystać tę klasę w metodzie
``load()`` w celu połączenia konfiguracji oraz wymuszenia walidacji. Jeśli przekazano
coś innego niż ``my_type``, użytkownik zostanie poinformowany wyjątkiem, że
przekazana opcja jest nieobsługiwana::

    public function load(array $configs, ContainerBuilder $container)
    {
        $configuration = new Configuration();

        $config = $this->processConfiguration($configuration, $configs);

        // ...
    }

Metoda ``processConfiguration()`` używa drzewa konfiguracji, które zdefiniowano
w klasie ``Configuration``, w celach walidacji, normalizacji oraz łączenia wszystkich
dostępnych tablic konfiguracji razem.

Klasa ``Configuration`` może być o wiele bardziej skomplikowana niż ta ukazana
tutaj, wspierając węzły tablic, węzły "prototypów", zaawansowaną walidację, normalizacje
specyficzne dla XMLa jak również zaawansowane połączenia. Można dowiedzieć się
o tym więcej czytając :doc:`the Config Component documentation</components/config/definition>`.
Można również zobaczyć to wszystko w akcji poprzez sprawdzenie głównych klas Configuration,
takich jak te z `konfiguracji FrameworkBundle`_ lub `konfiguracji TwigBundle`_.

Modyfikowanie konfiguracji innego pakietu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli kilkanaście pakietów zależy od siebie, może okazać się użyteczne, aby
umożliwić jednej klasie ``Extension`` modyfikowanie konfiguracji przekazywanej
do innej klasy ``Extension`` innego pakietu, tak jakby umożliwiając końcowemu programiscie
zamieszczenie jej w pliku ``app/config/config.yml``.

Aby uzyskać więcej informacji, zobacz :doc:`/cookbook/bundles/prepend_extension`.

Zrzut domyślnej konfiguracji
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Polecenie ``config:dump-reference`` umożliwia podejrzenie domyślnej konfiguracji
pakietu na wyjściu konsoli w formacie yaml.

Tak długo jak konfiguracja pakietu mieści się w standardowej lokalizacji
(``YourBundle\DependencyInjection\Configuration``) i nie posiada metody ``__construct()``,
wszystko będzie działać automatycznie. Jeśli cokolwiek odbiega od normy,
klasa ``Extension`` musi nadpisać metodę :method:`Extension::getConfiguration() <Symfony\\Component\\HttpKernel\\DependencyInjection\\Extension::getConfiguration>`,
a następnie zwrócić instancję klasy ``Configuration``.

Komentarze i przykłady mogą zostać dodane do wezłów konfiguracji z użyciem
metod ``->info()`` oraz ``->example()``::

    // src/Acme/HelloBundle/DependencyExtension/Configuration.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('acme_hello');

            $rootNode
                ->children()
                    ->scalarNode('my_type')
                        ->defaultValue('bar')
                        ->info('what my_type configures')
                        ->example('example setting')
                    ->end()
                ->end()
            ;

            return $treeBuilder;
        }
    }

Ten tekst pojawi się jako komentarz w formacie yaml po wydaniu polecenia ``config:dump-reference``.

.. index::
   pair: konwencje; konfiguracja

Konwencje rozszerzeń
--------------------

Podczas tworzenia klasy Extension, powinno się trzymać tych prostych konwencji:

* Rozszerzenie musi być zlokalizowane w podprzestrzeni nazw ``DependencyInjection``;

* Rozszerzenie musi być nazwane po nazwie pakietu i zakończone końcówką ``Extension``
  (``AcmeHelloExtension`` dla ``AcmeHelloBundle``);

* Rozszerzenie powinno dostarczać schemat XSD.

Jeśli stosuje się te proste konwencje, wszystkie rozszerzenia zostaną automatycznie
zarejestrowane przez Symfony2. Jeśli nie, należy przesłonić metodę
:method:`Bundle::build() <Symfony\\Component\\HttpKernel\\Bundle\\Bundle::build>`
w danym pakiecie::

    // ...
    use Acme\HelloBundle\DependencyInjection\UnconventionalExtensionClass;

    class AcmeHelloBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            // zarejestruj rozszerzenie, które nie trzyma się konwencji
            $container->registerExtension(new UnconventionalExtensionClass());
        }
    }

W tym przypadku, klasa Extension musi również implementować metodę ``getAlias()``
oraz zwracać unikalny alias stworzony na podstawie nazwy pakietu (np. ``acme_hello``).
Jest to wymagane, ponieważ nazwa klasy nie przestrzega jednej z norm, mianowicie
nie kończy się końcówką ``Extension``.

Dodatkowo metoda ``load()`` klasy Extension zostanie wywołana *tylko*, gdy
użytkownik określi alias ``acme_hello`` w przynajmniej jednym z plików konfiguracyjnych.
Dla przypomnienia, jest tak dlatego, ponieważ klasa Extension nie trzyma się
standardów określonych powyżej, zatem nic nie dzieje się automatycznie.

.. _`konfiguracji FrameworkBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/DependencyInjection/Configuration.php
.. _`konfiguracji TwigBundle`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/TwigBundle/DependencyInjection/Configuration.php
