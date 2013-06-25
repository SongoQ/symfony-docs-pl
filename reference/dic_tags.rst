.. highlight:: php
   :linenothreshold: 2

Tagi DI (wstrzykiwania zależności)
==================================

Tagi wstrzykiwania zależności, które tu nazywać też będziemy w skrócie tagami DI
(od ang. *Dependency Injection*), są krótkimi łańcuchami znaków, które mogą być
zastosowane do usług w celu ich oflagowania, tak by mogły być użyte w jakiś
specjalny sposób. Na przykład, jeśli mamy usługę, którą chcemy zarejestrować jako
detektor nasłuchujący (*ang. listener*) dla jakichś rdzennych zdarzeń Symfony,
to możemy ją oflagować tagiem ``kernel.event_listener``.

Można dowiedzieć się trochę więcej o "tagach" czytając rozdział
":ref:`book-service-container-tags`" części „Kontener usług” podręcznika Symfony.

Poniżej znajduje się informacja o wszystkich tagacg dostępnych w Symfony2
(rdzennych pakietach Symfony2). Tagi mogą być również używane przez inne pakiety
i nie są one tutaj wymienione.

+-----------------------------------+-----------------------------------------------------------------------+
| Nazwa tagu                        | Zastosowanie                                                          |
+-----------------------------------+-----------------------------------------------------------------------+
| `assetic.asset`_                  | Rejestruje :term:`aktywo <aktywa>` w bieżącym menadżerze aktywów      |
+-----------------------------------+-----------------------------------------------------------------------+
| `assetic.factory_worker`_         | Dodaje workera wytwórni (*ang. factory worker*)                       |
+-----------------------------------+-----------------------------------------------------------------------+
| `assetic.filter`_                 | Rejestruje filtr                                                      |
+-----------------------------------+-----------------------------------------------------------------------+
| `assetic.formula_loader`_         | Dodaje loadera Formula do bieżącego menadżera aktywów                 |
+-----------------------------------+-----------------------------------------------------------------------+
| `assetic.formula_resource`_       | Dodaje zasób do bieżącego menadżera aktywów                           |
+-----------------------------------+-----------------------------------------------------------------------+
| `assetic.templating.php`_         | Usuwa usługę, jeśli wyłączone jest szablonowanie php                  |
+-----------------------------------+-----------------------------------------------------------------------+
| `assetic.templating.twig`_        | Usuwa usługę, jeśli wyłączone jest szablonowanie twig                 |
+-----------------------------------+-----------------------------------------------------------------------+
| `data_collector`_                 | Tworzy klasę, która zbiera indywidualne dane dla profilera            |
+-----------------------------------+-----------------------------------------------------------------------+
| `doctrine.event_listener`_        | Dodaje detektor zdarzenia Doctrine                                    |
+-----------------------------------+-----------------------------------------------------------------------+
| `doctrine.event_subscriber`_      | Dodaje subskrybenta zdarzenia Doctrine                                |
+-----------------------------------+-----------------------------------------------------------------------+
| `form.type`_                      | Tworzy indywidualny typ pola formularza                               |
+-----------------------------------+-----------------------------------------------------------------------+
| `form.type_extension`_            | Tworzy własne "rozszerzenie formularza"                               |
+-----------------------------------+-----------------------------------------------------------------------+
| `form.type_guesser`_              | Dodaje własną logikę dla "zgadywanego typu formularza"                |
+-----------------------------------+-----------------------------------------------------------------------+
| `kernel.cache_clearer`_           | Rejestruje usługę dla wywoływania jej przy czyszczeniu pamięci podr.  |
+-----------------------------------+-----------------------------------------------------------------------+
| `kernel.cache_warmer`_            | Rejestruje usługę dla wywoływanie jej przy rozgrzewaniu pamięci podr. |
+-----------------------------------+-----------------------------------------------------------------------+
| `kernel.event_listener`_          | Nasłuchiwanie różnych zdarzeń (haków) w Symfony                       |
+-----------------------------------+-----------------------------------------------------------------------+
| `kernel.event_subscriber`_        | Do subskrybcji zbioru różnych zdarzeń (haków) w Symfony               |
+-----------------------------------+-----------------------------------------------------------------------+
| `kernel.fragment_renderer`_       | Dodaje nową strategię renderowania zawartości HTTP                    |
+-----------------------------------+-----------------------------------------------------------------------+
| `monolog.logger`_                 | Dla użycia indywidualnego kanału rejestrowania w Monolog              |
+-----------------------------------+-----------------------------------------------------------------------+
| `monolog.processor`_              | Dodaje indywidualny procesor dla rejestrowania w Monolog              |
+-----------------------------------+-----------------------------------------------------------------------+
| `routing.loader`_                 | Zarejestrowanie własnej usługi ładującej trasy                        |
+-----------------------------------+-----------------------------------------------------------------------+
| `security.voter`_                 | Dodanie indywidualnych voterów do logiki autoryzacji Symfony          |
+-----------------------------------+-----------------------------------------------------------------------+
| `security.remember_me_aware`_     | Umożliwienie obsługi "pamiętaj mnie" w autoryzacji                    |
+-----------------------------------+-----------------------------------------------------------------------+
| `serializer.encoder`_             | Zarejestrowanie nowego kodera w usłudze ``serializer``                |
+-----------------------------------+-----------------------------------------------------------------------+
| `serializer.normalizer`_          | Zarejestrowanie nowego normalizera w uśłudze ``serializer``           |
+-----------------------------------+-----------------------------------------------------------------------+
| `swiftmailer.plugin`_             | Zarejestrowanie indywidualnej wtyczki SwiftMailer                     |
+-----------------------------------+-----------------------------------------------------------------------+
| `templating.helper`_              | Udostępnienie usługi w szablonach PHP                                 |
+-----------------------------------+-----------------------------------------------------------------------+
| `translation.loader`_             | Zarejestrowanie indywidualnej usługi ładującej tłumaczenia            |
+-----------------------------------+-----------------------------------------------------------------------+
| `twig.extension`_                 | Zarejestrowanie indywidualnego rozszerzenia Twig                      |
+-----------------------------------+-----------------------------------------------------------------------+
| `twig.loader`_                    | Zarejestrowanie indywidualnej usługi ładującej szablony Twig          |
+-----------------------------------+-----------------------------------------------------------------------+
| `validator.constraint_validator`_ | Stworzenie indywidualnego ograniczenia walidacyjnego                  |
+-----------------------------------+-----------------------------------------------------------------------+
| `validator.initializer`_          | Zarejestrowanie usługi, która inicjuje obiekty przed walidacją        |
+-----------------------------------+-----------------------------------------------------------------------+

assetic.asset
-------------

**Przeznaczenie**: Rejestruje :term:`aktywo <aktywa>` w bieżącym menadżerze aktywów

assetic.factory_worker
----------------------

**Przeznaczenie**: Dodaje workera wytwórni

Worker wytwórni (*ang. factory worker*) jest klasą implementującą interfejs
``Assetic\Factory\Worker\WorkerInterface``.
Metoda ``process($asset)`` tej klasy jest wywoływana dla każdego aktywa po jego
utworzeniu. Można zmieniać aktywo lub nawet zwracać nowe.

W celu dodania nowego workera, najpierw trzeba stworzyć klasę::

    use Assetic\Asset\AssetInterface;
    use Assetic\Factory\Worker\WorkerInterface;

    class MyWorker implements WorkerInterface
    {
        public function process(AssetInterface $asset)
        {
            // ... change $asset or return a new one
        }

    }

i następnie dodać ją do rejestru jako oflagowaną tagiem usługę:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            acme.my_worker:
                class: MyWorker
                tags:
                    - { name: assetic.factory_worker }

    .. code-block:: xml
       :linenos:

        <service id="acme.my_worker" class="MyWorker>
            <tag name="assetic.factory_worker" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('acme.my_worker', 'MyWorker')
            ->addTag('assetic.factory_worker')
        ;

assetic.filter
--------------

**Przeznaczenie**: Rejestruje filtr

AsseticBundle używa tego tagu do zarejestrowania indywidualnych filtrów. Można
również użyć ten znacznik do zarejestrowania własnych filtrów.

Po pierwsze, trzeba utworzyć filtr::

    use Assetic\Asset\AssetInterface;
    use Assetic\Filter\FilterInterface;

    class MyFilter implements FilterInterface
    {
        public function filterLoad(AssetInterface $asset)
        {
            $asset->setContent('alert("yo");' . $asset->getContent());
        }

        public function filterDump(AssetInterface $asset)
        {
            // ...
        }
    }

Po drugie, trzeba zdefiniować usługę:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            acme.my_filter:
                class: MyFilter
                tags:
                    - { name: assetic.filter, alias: my_filter }

    .. code-block:: xml
       :linenos:

        <service id="acme.my_filter" class="MyFilter">
            <tag name="assetic.filter" alias="my_filter" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('acme.my_filter', 'MyFilter')
            ->addTag('assetic.filter', array('alias' => 'my_filter'))
        ;

Na koniec, zastosować filtr:

.. code-block:: jinja
   :linenos:

    {% javascripts
        '@AcmeBaseBundle/Resources/public/js/global.js'
        filter='my_filter'
    %}
        <script src="{{ asset_url }}"></script>
    {% endjavascripts %}

Można również zastosować fitr poprzez opcję konfiguracyjną
``assetic.filters.my_filter.apply_to``, tak jak opisano to w
":doc:`/cookbook/assetic/apply_to_option`". W tym celu należy zdefiniować usługę
filtrowania w oddzielnym pliku konfiguracyjnym xml i wskazać ten plik w kluczu
konfiguracyjnym ``assetic.filters.my_filter.resource``.

assetic.formula_loader
----------------------

**Przeznaczenie**: Dodaje loadera Formula do bieżącego menadżera aktywów

Loader Formula jest klasą implementującą interfejs
``Assetic\\Factory\Loader\\FormulaLoaderInterface``. Klasa ta jest odpowiedzialna
za ładowanie aktywów z odpowiedniego rodzaju zasobów (na przykład , szablon Twig).
Assetic dostarcza loaderów dla skryptów php i szablonów Twig.

Atrybut ``alias`` określa nazwę loadera.

assetic.formula_resource
------------------------

**Przeznaczenie**: Dodaje zasób do bieżącego menadżera aktywów.

Zasobem jest coś, co może być załadowane przez loadery Formula. Dla przykładu,
zasobami są szablony Twig.

assetic.templating.php
----------------------

**Przeznaczenie**: Usuwa usługi, jeśli wyłączone jest szablonowanie php.

Oznaczona tym tagiem usługa zostanie usunięta z kontenera, jeśli sekcja konfiguracji
``framework.templating.engines`` nie zawiera php.

assetic.templating.twig
-----------------------

**Przeznaczenie**: Usuwa usługi, jeśli wyłączone jest szablonowanie Twig.

Oznaczona tym tagiem usługa zostanie usunięta z kontenera, jeśli sekcja konfiguracji
``framework.templating.engines`` nie zawiera twig.

data_collector
--------------

**Przeznaczenie**: Tworzy klasę, która zbiera indywidualne dane dla profilera.

W celu poznania szczegółów o tworzeniu własnej, indywidualnej kolekcji danych,
przeczytaj artykuł ":doc:`/cookbook/profiler/data_collector`".

doctrine.event_listener
-----------------------

**Przeznaczenie**: Dodaje detektor zdarzeń Doctrine.

W celu poznania szczegółów o tworzeniu detektorów zdarzeń Doctrine, przeczytaj artykuł:
":doc:`/cookbook/doctrine/event_listeners_subscribers`".

doctrine.event_subscriber
-------------------------

**Przeznaczenie**: Dodaje subskrybenta zdarzeń Doctrine.

W celu poznania szczegółów o tworzeniu sybskrybenta zdarzeń Doctrine, przeczytaj artykuł:
":doc:`/cookbook/doctrine/event_listeners_subscribers`".

.. _dic-tags-form-type:

form.type
---------

**Przeznaczenie**: Tworzenie indywidualnego typu pola formularza.

W celu poznania szczegółów o tworzeniu własnych, indywidualnych typów formularzowych,
przeczytaj artykuł:
":doc:`/cookbook/form/create_custom_field_type`".

form.type_extension
-------------------

**Przeznaczenie**: Tworzenie indywidualnego "rozszerzenią typu pola formularza".

Rozszerzenia typu pola formularza, to sposób na utworzenie własnego typu pola
w formularzu. Na przykład, dodanie tokena CSRF jest wykonywane przez rozszerzenie
typu formularzowego
(:class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\FormTypeCsrfExtension`).

Rozszerzenie typu pola formularza mogą modyfikować każdą część dowolnego pola
w formularzu. Aby stworzyć rozszerzenie typu pola formularza, w pierwszej kolejności
należy zaimplementować interfejs :class:`Symfony\\Component\\Form\\FormTypeExtensionInterface`.
Dla uproszczenia najczęściej rozszerza się klasę :class:`Symfony\\Component\\Form\\AbstractTypeExtension`
zamiast bezpośrednio interfejs::

    // src/Acme/MainBundle/Form/Type/MyFormTypeExtension.php
    namespace Acme\MainBundle\Form\Type;

    use Symfony\Component\Form\AbstractTypeExtension;

    class MyFormTypeExtension extends AbstractTypeExtension
    {
        // ... fill in whatever methods you want to override
        // like buildForm(), buildView(), finishView(), setDefaultOptions()
    }

W celu poinformowania Symfony o rozszerzeniu typu pola formularza i zastosowania go,
należy przypisać mu tag `form.type_extension`:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            main.form.type.my_form_type_extension:
                class: Acme\MainBundle\Form\Type\MyFormTypeExtension
                tags:
                    - { name: form.type_extension, alias: field }

    .. code-block:: xml
       :linenos:

        <service id="main.form.type.my_form_type_extension" class="Acme\MainBundle\Form\Type\MyFormTypeExtension">
            <tag name="form.type_extension" alias="field" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('main.form.type.my_form_type_extension', 'Acme\MainBundle\Form\Type\MyFormTypeExtension')
            ->addTag('form.type_extension', array('alias' => 'field'))
        ;

Klucza ``alias`` w opcji ``tag`` jest typem pola, do którego rozszerzenie powinno
być zastosowane. Na przykład, aby zastosować rozszerzenie do każdego pola w formularzu,
użyj wartość "form".

form.type_guesser
-----------------

**Przeznaczenie**: Dodaje własną logikę do "zgadywanego typu pola formularza"

Tag pozwala dodać własną logikę dla przetwarzania
:ref:`zgadywanych typów pól<book-forms-field-guessing>`. Domyślnie zgadywanie typu
pola jest realizowany poprzez "interpretery" metadanych walidacyjnych i metadanych
Doctrine (jeżeli używa się Doctrine).

W celu dodania własnego interpretera , należy utworzyć klasę implementującą interfejs
:class:`Symfony\\Component\\Form\\FormTypeGuesserInterface`. Następnie otagować jej
definicję usługi tagiem ``form.type_guesser`` (to nie ma opcji).

Przykład tej klasy można zobaczyć oglądając klasę ``ValidatorTypeGuesser``
w komponencie ``Form``.

kernel.cache_clearer
--------------------

**Przeznaczenie**: Rejestruje usługę, aby była wywoływana podczas procesu czyszczenia
pamięci podręcznej

Czyszczenie pamięci podręcznej ma miejsce, gdy uruchamiana jest polecenie ``cache:clear``.
Jeżeli pakiet buforuje pliki, to można dodać własny kod czyszczący te pliki podczas
czysczenia pamięci.

W celu zarejestrowania własnego kodu czyszczącego, najpierw musi się utworzyć klasę
usługi::

    // src/Acme/MainBundle/Cache/MyClearer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheClearer\CacheClearerInterface;

    class MyClearer implements CacheClearerInterface
    {
        public function clear($cacheDir)
        {
            // clear your cache
        }

    }

Następnie trzeba zarejestrować tą klasę i oznaczyć ją tagiem ``kernel.cache:clearer``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            my_cache_clearer:
                class: Acme\MainBundle\Cache\MyClearer
                tags:
                    - { name: kernel.cache_clearer }

    .. code-block:: xml
       :linenos:

        <service id="my_cache_clearer" class="Acme\MainBundle\Cache\MyClearer">
            <tag name="kernel.cache_clearer" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('my_cache_clearer', 'Acme\MainBundle\Cache\MyClearer')
            ->addTag('kernel.cache_clearer')
        ;

kernel.cache_warmer
-------------------

**Przeznaczenie**: Rejestruje usługę aby wywołać ją przy rozgrzewaniu pamięci podręcznej.

Pamięć podręczna może być ciepła lub zimna. Gorąca pamięć zawiera dane, które są
aktywnie serwowane z pamięci podręcznej (na ogół dlatego, że są w użyciu).
Zimna pamięć nie zawiera danych, które są w danej chwili potrzebne. Rozgrzewanie
pamięci (*ang. cache warming process*) jest procesem przejścia od zimnej do ciepłej
pamięci w celu jej używania.

Rozgrzewanie pamięci podręcznej ma miejsce podczas uruchamiania z linii poleceń
zadań ``cache:warmup`` lub ``cache:clear`` (lecz nie wtedy, gdy w ``cache:clear``
przekazuje się opcje ``--no-warmup``). Celem jest zainicjowanie pamięci podręcznej,
która będzie potrzebna dla działania aplikacji i zabezpieczenia pierwszego użytkownika
przed jakimkolwiek istotnym "trafieniem w pamięć", gdzie pamięć jest generowana
dynamicznie.

Aby zarejestrować usługę rozgrzewającą (*ang. warmer*), najpierw musi sie utworzyć usługę
implementującą interfejs
:class:`Symfony\\Component\\HttpKernel\\CacheWarmer\\CacheWarmerInterface`::

    // src/Acme/MainBundle/Cache/MyCustomWarmer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheWarmer\CacheWarmerInterface;

    class MyCustomWarmer implements CacheWarmerInterface
    {
        public function warmUp($cacheDir)
        {
            // zrobienie jakiejś oparacji do "rozgrzania" pamięci podręcznej
        }

        public function isOptional()
        {
            return true;
        }
    }

Metoda ``isOptional`` zwraca ``true``, jeżeli możliwe jest użycie aplikacji bez
wywołania tego rozgrzewacza. W Symfony 2.0 usługi rozgrzewające mogą być ocjonalnie zawsze
wykonywane, więc ta funkcja nie ma realnego znaczenia.

Aby zarejestrować swoja usługę rozgrzewającą w Symfony, trzeba oznaczyć ją jako kernel.cache_warmer:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            main.warmer.my_custom_warmer:
                class: Acme\MainBundle\Cache\MyCustomWarmer
                tags:
                    - { name: kernel.cache_warmer, Priorytet: 0 }

    .. code-block:: xml
       :linenos:

        <service id="main.warmer.my_custom_warmer" class="Acme\MainBundle\Cache\MyCustomWarmer">
            <tag name="kernel.cache_warmer" Priorytet="0" />
        </service>

    .. code-block:: php
       :lienos:

        $container
            ->register('main.warmer.my_custom_warmer', 'Acme\MainBundle\Cache\MyCustomWarmer')
            ->addTag('kernel.cache_warmer', array('Priorytet' => 0))
        ;

Wartość ``Priorytet`` jest opcjonalna i wynosi 0. Wartość ta może zawierać się
pomiędzy -255 do 255 a usługi rozgrzewające będą wywoływane w kolejności ich priorytetów.

.. _dic-tags-kernel-event-listener:

kernel.event_listener
---------------------

**Przeznaczenie**: Nasłuchiwanie różnych zdarzeń (haków) w Symfony

Tag ten pozwala podłączyć swoją klasę w różnych punktach do przetwarzania Symfony.

W celu zobaczenia pełnego przykładu tego detektora, przeczytaj wpis
:doc:`/cookbook/service_container/event_listener`.

Kolejny praktyczny przykład detektora nasłuchującego kernel jest znajduje się w
artykule ":doc:`/cookbook/request/mime_type`".

Informacje o detektorze zdarzeń jądra
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Podczas dodawania własnych detektorów, może być przydatne dowiedzenie się o innych
detektorach nasłuchujących jądro Symfony i ich priorytetach.

.. note::

    Wszystkie detektory tutaj wymienione mogą nie działać w niektórych środowiskach
    i pakietach. Wykaz ten nie obejmuje detektorów używanych w pakietach
    osób trzecich.

kernel.request
..............

+-------------------------------------------------------------------------------+-----------+
| Nazwa klasy detektora                                                         | Priorytet |
+-------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`      | 1024      |
+-------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener` | 192       |
+-------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\SessionListener`     | 128       |
+-------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\RouterListener`        | 32        |
+-------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\LocaleListener`        | 16        |
+-------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\Security\\Http\\Firewall`                         | 8         |
+-------------------------------------------------------------------------------+-----------+

kernel.controller
.................

+--------------------------------------------------------------------------------+-----------+
| Nazwa klasy detektora                                                          | Priorytet |
+--------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\DataCollector\\RequestDataCollector` | 0         |
+--------------------------------------------------------------------------------+-----------+

kernel.response
...............

+-------------------------------------------------------------------------------------+-----------+
| Nazwa klasy detektora                                                               | Priorytet |
+-------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`                 | 0         |
+-------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`            | 0         |
+-------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\SecurityBundle\\EventListener\\ResponseListener`           | 0         |
+-------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`            | -100      |
+-------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`       | -128      |
+-------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener` | -128      |
+-------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\StreamedResponseListener`    | -1024     |
+-------------------------------------------------------------------------------------+-----------+

kernel.exception
................

+---------------------------------------------------------------------------+-----------+
| Nazwa klasy detektora                                                     | Priorytet |
+---------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`  | 0         |
+---------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener` | -128      |
+---------------------------------------------------------------------------+-----------+

kernel.terminate
................

+---------------------------------------------------------------------------------+-----------+
| Nazwa klasy detektora                                                           | Priorytet |
+---------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\SwiftmailerBundle\\EventListener\\EmailSenderListener` | 0         |
+---------------------------------------------------------------------------------+-----------+

.. _dic-tags-kernel-event-subscriber:

kernel.event_subscriber
-----------------------

**Przeznaczenie**: Do subskrypcji zbioru różnych zdarzeń (haków) w Symfony

Aby włączyć własnego subskrybenta, trzeba dodać go do zwykłej usługi w jednej ze
swoich konfiguracji oraz oznaczyć ją jako ``kernel.event_subscriber``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            kernel.subscriber.your_subscriber_name:
                class: Fully\Qualified\Subscriber\Class\Name
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml
       :linenos:

        <service id="kernel.subscriber.your_subscriber_name" class="Fully\Qualified\Subscriber\Class\Name">
            <tag name="kernel.event_subscriber" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('kernel.subscriber.your_subscriber_name', 'Fully\Qualified\Subscriber\Class\Name')
            ->addTag('kernel.event_subscriber')
        ;

.. note::

    Usługa musi implementować interfejs :class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface`.

.. note::

   Jeżeli usługa jest tworzona przez wytwórnię, **MUSI SIĘ** prawidłowo ustawić
   parametr ``class``, aby tag działał właściwie.

kernel.fragment_renderer
------------------------

**Przeznaczenie**: Dodaje nową strategię renderowania zawartości HTTP.

Aby dodać nową strategię renderowania – oprócz rdzennych strategii, takich jak
``EsiFragmentRenderer`` - trzeba utworzyć klasę implementującą interfejs
:class:`Symfony\\Component\\HttpKernel\\Fragment\\FragmentRendererInterface`,
rejestrując ją jako usługę, następnie onaczając ją jako ``kernel.fragment_renderer``.

.. _dic_tags-monolog:

monolog.logger
--------------

**Przeznaczenie**: Dla użycia indywidualnego kanału rejestrowania w Monolog

Monolog umożliwia współdzielenie swoich handlerów pomiędzy wiele kanałów rejestrowania.
Usługa rejestrowania używa kanału ``app``, ale można zmienić ten kanał podczas
wstrzykiwania rejestratora do usługi.

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: ["@logger"]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml
       :linenos:

        <service id="my_service" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php
       :linenos:

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('my_service', $definition);

.. note::

    Działa to tylko wtedy, gdy usługa rejestrowania jest argumentem konstruktora,
    a nie gdy jest wstrzykiwana poprzez settera.

.. _dic_tags-monolog-processor:

monolog.processor
-----------------

**Przeznaczenie**: Dodaje indywidualny procesor dla rejestrowania w Monolog

Monolog umożliwia dodawanie procesorów w rejestratorze lub w handlerach w celu
dodania dodatkowych danych w rekordach. Procesor przejmuje rekord jako argument
i musi zwrócić go po dodaniu jakichś dodatkowych danych w atrybucie ``extra``
rekordu.

Przyjrzyjmy się, jak można użyć wbudowanego ``IntrospectionProcessor`` dla dodania
pliku, linii, klasy i metody, gdy zostanie wyzwolony rejestrator.

Można dodać procesor globalnie:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor }

    .. code-block:: xml
       :linenos:

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" />
        </service>

    .. code-block:: php
       :linenos:

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor');
        $container->register('my_service', $definition);


.. tip::

    Jeśli usługa nie jest wywoływalna (stosując ``__invoke``), to można dodać w
    tagu atrybut ``method`` w celu zastosowania konkretnej metody.

Można również dodać procesor dla konkretnego handlera używając atrybutu ``handler``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, handler: firephp }

    .. code-block:: xml
       :linenos:

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" handler="firephp" />
        </service>

    .. code-block:: php
       :linenos:

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('handler' => 'firephp');
        $container->register('my_service', $definition);


Procesor może być również dodany do konkretnego kanału rejestracyjnego przez użycie
atrybutu ``channel``. Ten zabieg zarejestruje procesor tylko dla kanału rejestracyjnego
``security`` w komponencie Security:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, channel: security }

    .. code-block:: xml
       :linenos:

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" channel="security" />
        </service>

    .. code-block:: php
       :linenos:

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('channel' => 'security');
        $container->register('my_service', $definition);

.. note::

    Nie można stosować jednocześnie atrybutów ``handler`` i ``channel`` w tym samym
    tagu, ponieważ handlery są współdzielone pomiędzy wszystkimi kanałami.

routing.loader
--------------

**Przeznaczenie**: Zarejestrowanie indywidualnej usługi ładującej trasy

Aby udostępnić loader trasowania, trzeba dodać go jako zwykłą usługę w jednej ze
swoich konfiguracji i oznaczyć tagiem ``routing.loader``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            routing.loader.your_loader_name:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: routing.loader }

    .. code-block:: xml
       "linenos:

        <service id="routing.loader.your_loader_name" class="Fully\Qualified\Loader\Class\Name">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('routing.loader.your_loader_name', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('routing.loader')
        ;

Więcej informacji można znaleźć w :doc:`/cookbook/routing/custom_route_loader`.

security.remember_me_aware
--------------------------

**Przeznaczenie**: Umożliwienie obsługi "pamiętaj mnie" w autoryzacji

Tag jest używany wewnętrznie do udostępnienia funkcjonalności uwierzytelniania
„pamietaj mnie”. Jeśli ma się własną metodę uwierzytelniania typu „pamietaj mnie”,
to można potrzebować właśnie tego taga.

Jeśli własna wytwórnia uwierzytelniania rozszerza
:class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory`
i własny detektor uwierzytelniania rozszerza
:class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener`,
to w efekcie własny detektor uwierzytelniania będzie miał ten tag i będzie
funkcjonował automatycznie.

security.voter
--------------

**Przeznaczenie**: Dodanie indywidualnych voterów do logiki autoryzacji Symfony

Podczas wywołania ``isGranted`` w konekście systemu bezpieczeństwa Symfony, w tle
zostaje użyty system "voterów" w celu ustalenia, czy użytkownik powinien mieć dostęp.
Tag ``security.voter`` umożliwia dodanie do systemu własnego indywidualnego votera.

W celu uzyskania więcej informacji proszę przeczytać artykuł: :doc:`/cookbook/security/voters`.

.. _reference-dic-tags-serializer-encoder:

serializer.encoder
------------------

**Przeznaczenie**: Zarejestrowanie nowego kodera w usłudze ``serializer``

Klasa tak oznaczona powinna implementować interfejsy
:class:`Symfony\\Component\\Serializer\\Encoder\\EncoderInterface`
i :class:`Symfony\\Component\\Serializer\\Encoder\\DecoderInterface`.

Więcej szczegółów można znaleźć w :doc:`/cookbook/serializer`.

.. _reference-dic-tags-serializer-normalizer:

serializer.normalizer
---------------------

**Przeznaczenie**: Zarejestrowanie nowego normalizera w usłudze ``serializer``

Klasa oznaczona tym tagiem powinna implementować interfejsy
:class:`Symfony\\Component\\Serializer\\Normalizer\\NormalizerInterface`
i :class:`Symfony\\Component\\Serializer\\Normalizer\\DenormalizerInterface`.

Więcej szczegółów można znaleźć w :doc:`/cookbook/serializer`.

swiftmailer.plugin
------------------

**Przeznaczenie**: Rejestracja indywidualnej wtyczki SwiftMailer

Jeśli chce się używać indywidualnej wtyczki SwiftMailer, można zarejestrować ją
w SwiftMailer tworząc usługę dla tej wtyczki i oznaczając ją tagiem
``swiftmailer.plugin`` (nie ma on opcji).

Wtyczka SwiftMailer musi implementować interfejs ``Swift_Events_EventListener``.
Więcej informacji o wtyczkach można znaleźć w `dokumentacji wtyczek SwiftMailer`_.

Kilka wtyczek SwiftMailer znajduje się w rdzeniu Symfony i może być aktywowana
poprzez różną konfigurację. Ze szczegółami można zapoznać się w
:doc:`/reference/configuration/swiftmailer`.

templating.helper
-----------------

**Przeznaczenie**: Udostępnienie usługi w szablonach PHP

W celu udostępnienia indywidualnego helpera szablonowego, trzeba dodać go jako
zwykłą usługę w jednej ze swoich konfiguracji, oznakować tagiem ``templating.helper``
i określić atrybut ``alias`` (helper będzie dostępny w szablonach za pośrednictwem
tego aliasu):

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            templating.helper.your_helper_name:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml
       :linenos:

        <service id="templating.helper.your_helper_name" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('templating.helper.your_helper_name', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;

translation.loader
------------------

**Przeznaczenie**: Zarejestrowanie indywidualnej usługi ładującej tłumaczenia

Domyślnie tłumaczenia ładowane są z systemu plików w wielu różnych formatach
(YAML, XLIFF, PHP itd.). Jeśli zachodzi potrzeba załadowania tłumaczeń z innego
źródła, to najpierw trzeba utworzyć klasę implementującą interfejs
:class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface`::

    // src/Acme/MainBundle/Translation/MyCustomLoader.php
    namespace Acme\MainBundle\Translation;

    use Symfony\Component\Translation\Loader\LoaderInterface;
    use Symfony\Component\Translation\MessageCatalogue;

    class MyCustomLoader implements LoaderInterface
    {
        public function load($resource, $locale, $domain = 'messages')
        {
            $catalogue = new MessageCatalogue($locale);

            // jakiś kod ładujący trochę tłumaczeń z "zasobu"
            // następnie zapisanie tłumaczeń w katalogu
            $catalogue->set('hello.world', 'Hello World!', $domain);

            return $catalogue;
        }
    }


Własna metoda ładująca ``load`` jest odpowiedzialna za zwrócenie
:class:`Symfony\\Component\\Translation\\MessageCatalogue`.

Teraz rejestrujemy loadera jako usługę i oznaczamy tagiem ``translation.loader``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            main.translation.my_custom_loader:
                class: Acme\MainBundle\Translation\MyCustomLoader
                tags:
                    - { name: translation.loader, alias: bin }

    .. code-block:: xml
       :linenos:

        <service id="main.translation.my_custom_loader" class="Acme\MainBundle\Translation\MyCustomLoader">
            <tag name="translation.loader" alias="bin" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('main.translation.my_custom_loader', 'Acme\MainBundle\Translation\MyCustomLoader')
            ->addTag('translation.loader', array('alias' => 'bin'))
        ;

Opcja ``alias`` jest wymagana i bardzo ważna: określa "przyrostek" nazwy pliku,
który powinien być użyty dla plików zasobów wykorzystujących ten loader.
Na przykład załóżmy, ze mamy jakiś niestandardowy format ``bin``, który potrzebujemy
załadować. Jeśli mamy plik ``bin`` zawierający tłumaczenie polskie dla domeny
``messages``, to musimy mieć plik ``app/Resources/translations/messages.pl.bin``.

Kiedy Symfony próbuje załadować plik ``bin``, to przekazuje ścieżkę do naszego
loadera jako argument ``$resource``. Następnie można wykonać jakąś logikę potrzebną
do załadowania tłumaczeń.

Jeśli ładuje się tłumaczenia z bazy danych, to ciągle potrzebuje się pliku zasobu,
ale może on być pusty, albo zawierać jakąś małą porcję informacji o ładowaniu tych
zasobów z bazy danych. Plik jest kluczem do wyzwolenia metody ``load`` we własnym
loaderze.

.. _reference-dic-tags-twig-extension:

twig.extension
--------------

**Przeznaczenie**: Zarejestrowanie indywidualnego rozszerzenia Twig

W celu udostępnienia rozszerzenia Twig, trzeba dodać go jako zwykłą usługę w jednej
ze swoich konfiguracji i oznaczyć tagiem ``twig.extension``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            twig.extension.your_extension_name:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml
       :linenos:

        <service id="twig.extension.your_extension_name" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('twig.extension.your_extension_name', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

Aby uzyskać więcej informacji, o tym jak rzeczywiście utworzyć klasę rozszerzającą,
Twig proszę zapoznać się z `dokumentacją Twiga`_ w tym zakresie lub przeczytać artykuł:
:doc:`/cookbook/templating/twig_extension`.

Przed przystąpieniem do pisania własnych rozszerzeń, dobrze jest przejrzeć
`oficjalne repozytorium Twiga`_, w którym umieszczono już kilka przydatnych rozszerzeń.
Na przykład ``Intl`` i jego filtr ``localizeddate``, które formatują daty zgodnie
z ustawieniem regionalnym uzytkownika. Te oficjalne rozszerzenia Twiga również
dodaje się jak zwykłe usługi:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            twig.extension.intl:
                class: Twig_Extensions_Extension_Intl
                tags:
                    - { name: twig.extension }

    .. code-block:: xml
       :linenos:

        <service id="twig.extension.intl" class="Twig_Extensions_Extension_Intl">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('twig.extension.intl', 'Twig_Extensions_Extension_Intl')
            ->addTag('twig.extension')
        ;

twig.loader
-----------

**Przeznaczenie**: Zarejestrowanie indywidualnej usługi ładującej szablony Twig

Domyślnie Symfony używa tylko `loader Twig`_ -
:class:`Symfony\\Bundle\\TwigBundle\\Loader\\FilesystemLoader`.
Jeśli zachodzi konieczność ładowania szablonów Twiga z innego żródła, to można
utworzyć nowy loader i oznaczyć go tagiem ``twig.loader``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        services:
            acme.demo_bundle.loader.some_twig_loader:
                class: Acme\DemoBundle\Loader\SomeTwigLoader
                tags:
                    - { name: twig.loader }

    .. code-block:: xml
       ;linenos:

        <service id="acme.demo_bundle.loader.some_twig_loader" class="Acme\DemoBundle\Loader\SomeTwigLoader">
            <tag name="twig.loader" />
        </service>

    .. code-block:: php
       :linenos:

        $container
            ->register('acme.demo_bundle.loader.some_twig_loader', 'Acme\DemoBundle\Loader\SomeTwigLoader')
            ->addTag('twig.loader')
        ;

validator.constraint_validator
------------------------------

**Przeznaczenie**: Stworzenie indywidualnego ograniczenia walidacyjnego

Tag umożliwia utworzenie i zarejestrowanie własnego indywidualnego ograniczenia
walidacyjnego. W celu uzyskania więcej informacji, przeczytaj artykuł:
:doc:`/cookbook/validation/custom_constraint`.

validator.initializer
---------------------

**Przeznaczenie**: Zarejestrowanie usługi, która inicjuje obiekty przed walidacją

Tag zapewnia bardzo niezwykłą porcję funkcjonalności, umożliwiającą wykonywanie
kilku różnego rodzaju działań na obiekcie przed jego walidacją. Na przykład, jest
używany przez Doctrine w zapytaniu o wszystkie „leniwie ładowane” dane w
obiekcie przed jego walidacją. Bez tego niektóre dane w encji Doctrine będą
podczas walidacji traktowane jako "brakujące", choć nie jest to prawdziwe.

Jeśli zachodzi potrzeba użycia tego tagu, to wystarczy stworzyć nową klasę
implementujacą interfejs
:class:`Symfony\\Component\\Validator\\ObjectInitializerInterface`.
Następnie oznaczyć ją tagiem ``validator.initializer`` (nie ma on opcji).

Dla przykładu przyjrzyj się klasie ``EntityInitializer`` wewnątrz Doctrine Bridge.

.. _`dokumentacją Twiga`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`oficjalne repozytorium Twiga`: https://github.com/fabpot/Twig-extensions
.. _`KernelEvents`: https://github.com/symfony/symfony/blob/2.2/src/Symfony/Component/HttpKernel/KernelEvents.php
.. _`dokumentacji wtyczek SwiftMailer`: http://swiftmailer.org/docs/plugins.html
.. _`Twig Loader`: http://twig.sensiolabs.org/doc/api.html#loaders
