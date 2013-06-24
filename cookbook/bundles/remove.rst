.. index::
    single: pakiet; usuwanie AcmeDemoBundle

Jak usunąć AcmeDemoBundle
=========================

Standardowa Edycja Symfony2 wyposażona jest w kompletne demo, które umieszczono
w pakiecie o nazwie ``AcmeDemoBundle``. Jest to szkielet, na którym można
się wzorować rozpoczynając nowe projekty, ale prędzej czy później przyjdzie pora,
aby go usunąć.

.. tip::

    Ten artykuł korzysta z przykładowego pakietu ``AcmeDemoBundle``, nic jednak
    nie stoi na przeszkodzie, aby zastosować użyte tu kroki do usunięcia
    dowolnego innego pakietu.

1. Wyrejestrowanie pakietu w ``AppKernel``
------------------------------------------

Aby odłączyć pakiet od frameworka, powinno się go usunąć z metody ``Appkernel::registerBundles()``.
Pakiet można zazwyczaj odszukać w tablicy ``$bundle``, zważ jednak, że ``AcmeDemoBundle``
został zarejestrowany tylko w środowisku deweloperskim, przez co można go odnaleźć w
instrukcji if po::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(...);

            if (in_array($this->getEnvironment(), array('dev', 'test'))) {
                // comment or remove this line:
                // $bundles[] = new Acme\DemoBundle\AcmeDemoBundle();
                // ...
            }
        }
    }

2. Usuwanie konfiguracji pakietu
--------------------------------

Teraz, gdy Symfony przestało interesować się pakietem, trzeba jeszcze usunąć
wszelkie ustawienia i konfiguracje routingu z katalogu ``app/config``, które
odnoszą się do tego pakietu.

2.1 Usuwanie routingu w pakiecie
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Routing dla pakietu AcmeDemoBundle można znaleźć w ``app/config/routing_dev.yml``.
Jedyne co trzeba zrobić, to usunąć wpis ``_acme_demo`` na dole tego pliku.

2.2 Usuwanie konfiguracji pakietu
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Niektóre pakiety zawierają konfigurację w jednym z plików ``app/config/config*.yml``.
Należy pamiętać, żeby usunąć z nich tylko powiązane ustawienia. W tym
celu wystarczy szukać nazwy ``acme_demo`` (lub jakiejkolwiek innej nazwy
pakietu, np. ``fos_user`` dla ``FOSUserBundle``) w plikach konfiguracyjnych.

Pakiet ``AcmeDemoBundle`` nie posiada konfiguracji, aczkolwiekkże jest używany
w pliku konfiguracyjnym ``app/config/security.yml``. Plik ten można traktować jako
wzór dla swojej konfiguracji security, jednak równie dobrze **można** usunąć z niego
wszystko: nie ma to bowiem żadnego znaczenia dla Symfony.

3. Usuwanie pakietu z systemu plików
------------------------------------

Z aplikacji zostały już usunięte wszystkie odniesienia do pakietu, najwyższy
czas, aby usunąć go również z systemu plików. Pakiet znajduje się w katalogu
``src/Acme/DemoBundle``. Należy usunąć ten katalog jak również katalog ``Acme``.

.. tip::
    Jeśli nie zna się położenia pakietu, można użyć metody
    :method:`Symfony\\Bundle\\FrameworkBundle\\Bundle\\Bundle::getPath`, w
    celu uzyskania ścieżki do tego pakietu::

        echo $this->container->get('kernel')->getBundle('AcmeDemoBundle')->getPath();

4. Usuwanie zależności w innych pakietach
-----------------------------------------

.. note::

    Nie dotyczy to pakietu ``AcmeDemoBundle`` - żaden inny pakiet nie zależy
    od niego, można więc pominąć ten krok.

Niektóre pakiety zależą od innych, jeśli usunie się jeden z nich, inny
prawdopodobnie przestanie działać. Upewnij się, że żadne inne pakiety, zarówno
firm trzecich jak i własne, nie zależą od pakietu, który zamierzasz usunąć.

.. tip::

    Jeśli jeden pakiet zależy od drugiego, w większości oznacza to, że wykorzystuje
    on pewne usługi z tego drugiego. Wyszukiwanie nazwy ``acme_demo`` powinno
    pomóc odnaleźć wszystkie zależności.

.. tip::

    Jeśli pakiet firm trzecich zależy od innego pakietu, można dowiedzieć się
    tego z dołączonego pliku ``composer.json``, który powinien znajdować się
    w katalogu tego pakietu.
