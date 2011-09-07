Tagi Wstrzykiwania Zależności (Dependency Injection)
====================================================

Tagi:

* ``data_collector``
* ``form.type``
* ``form.type_extension``
* ``form.type_guesser``
* ``kernel.cache_warmer``
* ``kernel.event_listener``
* ``monolog.logger``
* ``monolog.processor``
* ``templating.helper``
* ``routing.loader``
* ``translation.loader``
* ``twig.extension``
* ``validator.initializer``

Włączenie Niestandardowych Helperów Szablonów PHP
-------------------------------------------------

Aby włączyć niestandardowy helper szablonu, dodaj go jako
regularną usługę w swojej konfiguracji, otaguj go tagiem 
``templating.helper`` oraz zdefiniuj atrybut ``alias`` (helper
będzie dostępny poprzez ten alias w szablonach):

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.your_helper_name:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.your_helper_name" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.your_helper_name', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;

Włączenie Niestandardowych Rozszerzeń Twig
--------------------------------------------

Aby włączyć rozszerzenie Twig, dodaj go jako regularną usługę
w swojej konfiguracji, oraz otaguj go tagiem ``twig.extension``:

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.your_extension_name:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.your_extension_name" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.your_extension_name', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

.. _dic-tags-kernel-event-listener:

Włączenie Niestandardowych Nasłuchiwaczy (Listeners)
----------------------------------------------------

Aby włączyć niestandardowego słuchacza (listener), dodaj go jako regularną
usługę w swojej konfiguracji, oraz otaguj tagiem ``kernel.event_listener``.
Musisz ustawić nazwę zdarzenia (event) które Twoja będzie nasłuchiwała,
oraz nazwę metody która zostanie wywołana:

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.listener.your_listener_name:
                class: Fully\Qualified\Listener\Class\Name
                tags:
                    - { name: kernel.event_listener, event: xxx, method: onXxx }

    .. code-block:: xml

        <service id="kernel.listener.your_listener_name" class="Fully\Qualified\Listener\Class\Name">
            <tag name="kernel.event_listener" event="xxx" method="onXxx" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.listener.your_listener_name', 'Fully\Qualified\Listener\Class\Name')
            ->addTag('kernel.event_listener', array('event' => 'xxx', 'method' => 'onXxx'))
        ;

.. note::

    Możesz także ustawić priorytet jako liczba dodatnia lub też ujemna,
    opcja ta umożliwia Ci ustawienie czy słuchacz (listener) będzie zawsze uruchamiany
    przed lub też po innym słuchaczu.


Włączenie Niestandardowych Silników Szablonów
---------------------------------------------

Aby włączyć niestandardowy silnik szablonu, dodaj go jako regularną usługę
w swojej konfiguracji, oraz otaguj ją tagiem ``templating.engine``:

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.engine.your_engine_name:
                class: Fully\Qualified\Engine\Class\Name
                tags:
                    - { name: templating.engine }

    .. code-block:: xml

        <service id="templating.engine.your_engine_name" class="Fully\Qualified\Engine\Class\Name">
            <tag name="templating.engine" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.engine.your_engine_name', 'Fully\Qualified\Engine\Class\Name')
            ->addTag('templating.engine')
        ;

Włączenie Niestandardowego Loadera Routingu
-------------------------------------------

Aby włączyć niestandardowy loader routingu, dodaj go jako regularną usługę
w swojej konfiguracji, oraz otaguj ją tagiem ``routing.loader``:

.. configuration-block::

    .. code-block:: yaml

        services:
            routing.loader.your_loader_name:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <service id="routing.loader.your_loader_name" class="Fully\Qualified\Loader\Class\Name">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('routing.loader.your_loader_name', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('routing.loader')
        ;

.. _dic_tags-monolog:

Korzystanie z niestandardowego logowania z Monolog
--------------------------------------------------

Monolog umożliwia działanie na kilku kanałach logowania.
Usługa "logger" używa kanału ``app`` ale możesz zmienić ten kanał
gdy wstrzykujesz (dependency injection) logera w usługę.

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: [@logger]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="my_service" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('my_service', $definition);;

.. note::

    Działa to tylko wtedy gdy usługa logera jest przekazywana jako argument konstruktora,
    sposób ten nie zadziała gdy jest wstrzykiwany przez settera.

.. _dic_tags-monolog-processor:

Dodawanie procesorów dla Monolog
--------------------------------

Monolog daje nam możliwość na dodawanie procesorów w logerze lub też w obsługach
dodawania dodatkowych danych do rekordów. Procesor otrzymuje rekord jako argument
oraz musi go zwrócić po dodaniu dodatkowych informacji w atrybucie ``extra``.

Zobacz jak możesz użyć wbudowanego procesora ``IntrospectionProcessor`` aby dodać
plik, linię, klasę oraz metodę gdzie loger został włączony.

Możesz dodać procesor globalnie:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor');
        $container->register('my_service', $definition);

.. tip::

    Jeśli Twojej usługi nie da się wywołać jak funkcji (poprzez ``__invoke``)
    możesz dodać atrybut ``method`` w tagu do użycia określonej metody.

Możesz także dodać procesor dla określonej obsługi używając atrybutu ``handler``:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, handler: firephp }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" handler="firephp" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('handler' => 'firephp');
        $container->register('my_service', $definition);

Możesz także dodać procesor dla określonego kanału logowania używając atrybutu
``channel``. Poniższy przykład zarejestruje procesor tylko dla kanału logowania
``security`` użytego w komponencie Security:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, channel: security }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" channel="security" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('channel' => 'security');
        $container->register('my_service', $definition);

.. note::

    Nie możesz używać atrybutów ``handler`` oraz ``channel`` naraz dla tego samego
    tagu ponieważ obsługi dzielone są pomiędzy wszystkimi kanałami.
