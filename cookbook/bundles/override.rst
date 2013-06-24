.. index::
   single: pakiet; dziedziczenie

Jak zastąpić każdą część pakietu
================================

Ten dokument to szybkie odniesienie się do sposobów przesłaniania różnych
części pakietów firm trzecich.

Szablony
--------

Aby uzyskać informacje o sposobach przesłaniania szablonów, zobacz:

* :ref:`overriding-bundle-templates`.
* :doc:`/cookbook/bundles/inheritance`

Trasowanie
----------

Trasowanie nie jest automatycznie importowane z Symfony2. Jeśli chciałoby się
dołączyć trasy z dowolnego pakietu, należy je ręcznie zaimportować do
swojej aplikacji (np. ``app/config/routing.yml``).

Najprostszym sposobem na "zastąpienie" trasowania w pakiecie jest nie importowanie
go wcale. Zamiast tego, odpowiedniejszym wydaje się skopiowanie pliku trasowania
z danego pakietu do swojej aplikacji, zmodyfikowanie go, a dopiero potem
zaimportowanie.

Kontrolery
----------

Zakładając, że używane pakiety firm trzecich stosują kontrolery "non-service"
(co zdarza się w większości przypadków), można je bardzo łatwo zastąpić
dzięki dziedziczeniu pakietu. Aby uzyskać więcej informacji, zobacz
:doc:`/cookbook/bundles/inheritance`. Jeśli kontroler jest usługą (ang. service),
zobacz następny rozdział, aby dowiedzieć się jak postąpić w tym przypadku.

Usługi i konfiguracja
---------------------

W celu przesłonięcia lub rozszerzenia kodu usługi można skorzystać z dwóch opcji. Po
pierwsze można ustawić parametr zawierający nazwę klasy usługi do swoich klas
modyfikując ustawienia w ``app/config/config.yml``. To jest oczywiście możliwe
tylko wtedy, gdy nazwa klasy jest zdefiniowana jako parametr w konfiguracji usługi
pakietu zawierającego tę usługę. Na przykład, aby zastąpić klasę używaną w
Symfonowej usłudze ``translator``, należałoby przesłonić parametr ``translator.class``.
Upewnienie się, który parametr przesłonić nieraz może zająć naprawdę sporo czasu. Dla
translatora jest on zdefiniowany i używany w pliku ``Resources/config/translation.xml``
głównego pakietu FrameworkBundle:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        parameters:
            translator.class:      Acme\HelloBundle\Translation\Translator

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="translator.class">Acme\HelloBundle\Translation\Translator</parameter>
        </parameters>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->setParameter('translator.class', 'Acme\HelloBundle\Translation\Translator');

Po drugie, jeśli klasa nie jest dostępna jako parametr, należy upewnić się,
że będzie zawsze przesłaniana, tudzież powinno się zmodyfikować coś więcej
niż tylko jej nazwę, w typ przypadku z użyciem interfejsu compiler pass::

    // src/Acme/DemoBundle/DependencyInjection/Compiler/OverrideServiceCompilerPass.php
    namespace Acme\DemoBundle\DependencyInjection\Compiler;

    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class OverrideServiceCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            $definition = $container->getDefinition('original-service-id');
            $definition->setClass('Acme\DemoBundle\YourService');
        }
    }

W tym przykładzie pobiera się definicję oryginalnej usługi i ustawia jej
nazwę klasy na swoją.

Zobacz :doc:`/cookbook/service_container/compiler_passes` w celu zasięgnięcia
informacji na temat używania compiler pass. Jeśli chce się zrobić coś poza
przesłanianiem klasy - jak choćby dodać wywołania metody - jedyne co można
zrobić, to skorzystać z metod compiler pass.

Encje i mapowanie encji
-----------------------

Z uwagi na to jak działa Doctrine, nie jest możliwe zastąpienie mapowania
encji w pakiecie. Jednakże, jeśli pakiet dostarcza odwzorowaną superklasę (jak
encja ``User`` w pakiecie FOSUserBundle), możliwe jest zastąpienie jej atrybutów
i powiązań. Dowiedz się więcej o tej funkcjonalności i jej ograniczeniach
czytając `dokumentację Doctrine`_.

Formularze
----------

Aby zastąpić typ formularza, musi być on zarejestrowany jako usługa (czyli
przy użyciu etykiety "form.type"). Można wówczas zastąpić go tak jak każdą
inną usługę, co zostało szczegółowo wyjaśnione w dziale `Usługi & konfiguracja`_.
To oczywiście zadziała tylko wtedy, gdy typ formularza został zdefiniowany
aliasem, a nie przez utworzenie egzemplarza klasy, np.::

    $builder->add('name', 'custom_type');

zamiast::

    $builder->add('name', new CustomType());

Walidacja metadanych
--------------------

W toku...

.. _override-translations:

Tłumaczenia
-----------

Tłumaczenia nie są powiązane z pakietami, tylko z domenami. Oznacza to, że
można je zastąpić dowolnym plikiem tłumaczeń, o ile znajduje się w
:ref:`odpowiedniej domenie <translation-domains>`.

.. caution::

    Ostatni plik tłumaczeń zawsze wygrywa. Oznacza to, że trzeba upewnić
    się czy pakiet zawierający *twoje* tłumaczenia jest ładowany na samym
    końcu, zaraz po wszystkich tłumaczeniach, które próbowano przesłonić.
    Jest to robione w ``AppKernel``.

    Plik, który zawsze wygrywa to ten, który umieszczono w katalogu
    ``app/Resources/translations``, gdyż pliki z tego katalogu są zawsze
    wczytywane na samym końcu.

.. _`dokumentację Doctrine`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/inheritance-mapping.html#overrides
