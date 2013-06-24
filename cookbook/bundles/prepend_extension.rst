.. index::
   single: konfiguracja; semantyczny
   single: pakiet; konfiguracja rozszerzenia

Jak uprościć konfiguracje wielu pakietów
========================================

Budując aplikacje rozszerzalne i przeznaczone dla wielu użytkowników, programiści
stają w obliczu wyboru: albo utworzyć jeden, wielki pakiet, albo też kilkanaście mniejszych.
Tworzenie pojedynczego pakietu ma ten minus, że uniemożliwia użytkownikom
zrezygnowanie z funkcjonalności, której nie chcą używać. Z kolei tworzenie
wielu pakietów ma tą wadę, że konfiguracja staję się coraz bardziej uciążliwa,
a poszczególne opcje muszą być powielane w różnych pakietach.

Korzystając z metody poniżej, możliwe jest zniwelowanie wad korzystania z
wielu pakietów dzięki stworzeniu pojedynczego rozszerzenia (ang. Extension),
które umożliwi dołączenie opcji do każdego z pakietów. Rozszerzenie może
korzystać z opcji określonych w ``app/config/config.yml``, a następnie
dołączać je tak jak gdyby były jawnie napisane w konfiguracji aplikacji.

Przykładowo można by tego użyć do skonfigurowania nazwy menedżera encji,
która byłaby widoczna w kilku pakietach jednocześnie. Albo też do ustawienia
opcjonalnej właściwości, która zależałaby od innego, wczytywanego pakietu.

Aby rozszerzenie mogło to zrobić, musi implementować klasę
:class:`Symfony\\Component\\DependencyInjection\\Extension\\PrependExtensionInterface`::

    // src/Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\HttpKernel\DependencyInjection\Extension;
    use Symfony\Component\DependencyInjection\Extension\PrependExtensionInterface;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeHelloExtension extends Extension implements PrependExtensionInterface
    {
        // ...

        public function prepend(ContainerBuilder $container)
        {
            // ...
        }
    }

Wewnątrz metody :method:`Symfony\\Component\\DependencyInjection\\Extension\\PrependExtensionInterface::prepend` programiści mają pełny dostęp do instancji klasy
:class:`Symfony\\Component\\DependencyInjection\\ContainerBuilder`, tuż zanim
metoda :method:`Symfony\\Component\\DependencyInjection\\Extension\\ExtensionInterface::load`
zostanie wywołana na każdym z zarejestrowanych rozszerzeń pakietu. W celu
dołączenia ustawień do rozszrzenia pakietu, programiści mogą użyć metody
:method:`Symfony\\Component\\DependencyInjection\\ContainerBuilder::prependExtensionConfig`
obecnej w instancji klasy :class:`Symfony\\Component\\DependencyInjection\\ContainerBuilder`.
Ponieważ metoda ta tylko dołącza ustawienia, wszelkie inne ustawienia, robione
bezpośrednio wewnątrz ``app/config/config/yml`` nadpiszą te, które zostały
dołączone.

Poniższy przykład ilustruje jak dołączyć ustawienia konfiguracji w wielu
pakietach oraz jak wyłączyć flagi na wypadek gdyby dany pakiet nie był
zarejestrowany::

    public function prepend(ContainerBuilder $container)
    {
        // pobierz wszystkie pakiety
        $bundles = $container->getParameter('kernel.bundles');
        // określ czy pakiet AcmeGoodbyeBundle jest zarejestrowany
        if (!isset($bundles['AcmeGoodbyeBundle'])) {
            // wyłącz AcmeGoodbyeBundle w pakietach
            $config = array('use_acme_goodbye' => false);
            foreach ($container->getExtensions() as $name => $extension) {
                switch ($name) {
                    case 'acme_something':
                    case 'acme_other':
                        // ustaw use_acme_goodbye na false w konfiguracji acme_something oraz acme_other
                        // zauważ, że jeśli użytkownik ręcznie ustawił use_acme_goodbye na true w
                        // app/config/config.yml, to ustawienie to ostatecznie będzie true, a nie false
                        $container->prependExtensionConfig($name, $config);
                        break;
                }
            }
        }

        // przetwórz konfigurację rozszerzenia AcmeHelloExtension
        $configs = $container->getExtensionConfig($this->getAlias());
        // użyj klasy Configuration aby wygenerować tablicę konfiguracji z opcją ``acme_hello``
        $config = $this->processConfiguration(new Configuration(), $configs);

        // sprawdź czy entity_manager_name jest ustawione w konfiguracji ``acme_hello``
        if (isset($config['entity_manager_name'])) {
            // dołącz ustawienie acme_something do entity_manager_name
            $config = array('entity_manager_name' => $config['entity_manager_name']);
            $container->prependExtensionConfig('acme_something', $config);
        }
    }

Ekwiwalent powyższego można dodać do ``app/config/config.yml`` w sytuacji,
gdy ``AcmeGoodbyeBundle`` nie jest zarejestrowane, a opcja ``entity_manager_name``
dla ``acme_hello`` ustawiona na ``non_default``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml

        acme_something:
            # ...
            use_acme_goodbye: false
            entity_manager_name: non_default

        acme_other:
            # ...
            use_acme_goodbye: false

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->

        <acme-something:config use-acme-goodbye="false">
            <acme-something:entity-manager-name>non_default</acme-something:entity-manager-name>
        </acme-something:config>

        <acme-other:config use-acme-goodbye="false" />

    .. code-block:: php
       :linenos:

        // app/config/config.php

        $container->loadFromExtension('acme_something', array(
            ...,
            'use_acme_goodbye' => false,
            'entity_manager_name' => 'non_default',
        ));
        $container->loadFromExtension('acme_other', array(
            ...,
            'use_acme_goodbye' => false,
        ));

