.. highlight:: php
   :linenothreshold: 2


.. index::
   single: pakiety; dziedziczenie

"Jak wykorzystywać dziedziczenie pakietów w celu zastępowania części pakietu
============================================================================

Podczas pracy z pakietami osób trzecich, najprawdopodobnie nadejdzie moment,
w którym potrzeba będzie zastąpić plik z zewnętrznego pakietu plikiem
obecnym we własnym pakiecie. Symfony umożliwia w bardzo wygodny sposób przesłanianie
kontrolerów, szablonów i innych plików obecnych w katalogu ``Resources/``.

Załóżmy na przykład, że instalując pakiet `FOSUserBundle`_ następuje potrzeba
zastąpienia jego bazowego szablonu ``layout.html.twig`` oraz jego kontrolerów.
Załóżmy także, że powstanie lokalny pakiet ``AcmeUserBundle``, gdzie będą
trzymywane wszystkie przesłonięte pliki. By rozpocząć, należy zarejestrować
pakiet ``FOSUserBundle`` jako "rodzica" pakietu ``AcmeUserBundle``::

    // src/Acme/UserBundle/AcmeUserBundle.php
    namespace Acme\UserBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeUserBundle extends Bundle
    {
        public function getParent()
        {
            return 'FOSUserBundle';
        }
    }

Poprzez tą prostą zmianę, można teraz zastąpić kilka części pakietu ``FOSUserBundle``
tworząc plik o identycznej nazwie.

.. note::

    Pomimo nazwy metody, nie ma relacji "rodzic-dziecko" pomiędzy pakietami,
    jest to tylko sposób na rozszerzenie i zastąpienie istniejącego pakietu.

Przesłanianie kontrolerów
~~~~~~~~~~~~~~~~~~~~~~~~~

Załóżmy, że trzeba dodać funkcjonalność do akcji ``registerAction`` z kontrolera
``RegistrationController``, który mieści się w pakiecie ``FOSUserBundle``. Aby to
zrobić, należy stworzyć swój plik kontrolera ``RegistrationController.php``,
zastąpić oryginalną metodę pakietu ``FOSUserBundle``, a następnie zmienić jej
funkcjonalność::

    // src/Acme/UserBundle/Controller/RegistrationController.php
    namespace Acme\UserBundle\Controller;

    use FOS\UserBundle\Controller\RegistrationController as BaseController;

    class RegistrationController extends BaseController
    {
        public function registerAction()
        {
            $response = parent::registerAction();

            // ... do custom stuff
            return $response;
        }
    }

.. tip::

    W zależności od tego, jak bardzo trzeba zmienić konkretne zachowanie,
    można wywołać ``parent::registerAction()``, albo też całkowicie zastąpić jego
    logike swoją własną.

.. note::

    Przesłanianie w ten sposób kontrolerów zadziały tylko, gdy pakiet
    dotyczy kontrolera używającego standardowej składni ``FOSUserBundle::Registration::register``
    dla tras i szablonów. Jest to najlepsza praktyka.

Przesłanianie zasobów: szablony, trasowanie, walidacja, itd.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Większość zasobów może zostać przesłonięta, dzięki prostemu stworzeniu pliku w
tej samej lokakalizacji co w pakiecie nadrzędnym.

Bardzo często zdarza się, że trzeba zastąpić szablon ``layout.html.twig`` z
pakietu ``FOSUserBundle``, tak, aby używał głównego układu lokalnej aplikacji.
Ponieważ plik ten mieści się wewnątrz pakietu w ``Resources/views/layout.html.twig``
``FOSUserBundle``, można utworzyc swój plik w tej samej lokalizacji, tyle że w
pakiecie ``AcmeUserBundle``. Symfony zignoruje wówczas plik, który mieści się
wewnątrz pakietu ``FOSUserBundle``, a użyje zamiast tego pliku z lokalnego
pakietu ``AcmeUserBundle``.

To samo dotyczy plików trasowania, walidacji, konfiguracji i innych zasobów.

.. note::

    Przesłanianie zasobów zadziała tylko wtedy, gdy odnoszono się do nich z
    użyciem metody ``@FosUserBundle/Resources/config/routing/security.xml``.
    Jeśli odnoszono się do nich bez użycia skrótu @BundleName, wówczas nie
    zostaną one zastąpione w ten sposób.

.. caution::

    Pliki tłumaczeń nie działają w ten sam sposób, jak opisano powyżej.
    Przeczytaj :ref:`override-translations`, jeśli chcesz dowiedzieć się jak
    zastąpić tłumaczenia.

.. _`FOSUserBundle`: https://github.com/friendsofsymfony/fosuserbundle
