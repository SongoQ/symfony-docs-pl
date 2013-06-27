.. index::
   single: pakiet; najlepsze praktyki

Jak stosować najlepsze praktyki w strukturyzowaniu pakietów
===========================================================

Pakiet to katalog posiadający dobrze zdefiniowaną strukturę, mogący obsługiwać
wszystko, począwszy od klas, a skończywszy na kontrolerach i zasobach internetowych.
Chociaż pakiety są bardzo elastyczne, należy stosować się do najlepszych
praktyk, aby zacząć je z powodzeniem rozpowszechniać.

.. index::
   pair: pakiet; konwencje nazewnictwa

.. _bundles-naming-conventions:

Nazwa pakietu
-------------

Pakiet to również przestrzeń nazw PHP. Przestrzeń ta musi przestrzegać
zasad żądzących `standardami`_ w PHP 5.3 dla przestrzeni nazw oraz nazewnictwa
klas: rozpoczyna się od segmentu dostawcy, następnie zawiera zero lub więcej
segmentów kategorii, by zakończyć się krótka nazwą przestrzeni nazw, która
musi koniecznie kończyć się końcówką ``Bundle``.

Przestrzeń nazw staję się pakietem w chwili, gdy doda się do niej klasę
pakietu. Nazwa klasy pakietu musi przestrzegać następujących, prostych reguł:

* Używać tylko znaków alfanumerycznych i podkreśleń;
* Używać nazewnictwa w stylu CamelCase;
* Używać treściwych i krótkich nazw (nie więcej niż 2 słowa);
* Posiadać przedrostek zawierający nazwę dostawcy (i opcjonalnie nazwę
  przestrzeni);
* Posiadać końcówkę ``Bundle``.

Oto kilka poprawnych przestrzeni pakietów i nazw klas:

+-----------------------------------+--------------------------+
| Przestrzeń nazw                   | Nazwa klasy pakietu      |
+===================================+==========================+
| ``Acme\Bundle\BlogBundle``        | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+
| ``Acme\Bundle\Social\BlogBundle`` | ``AcmeSocialBlogBundle`` |
+-----------------------------------+--------------------------+
| ``Acme\BlogBundle``               | ``AcmeBlogBundle``       |
+-----------------------------------+--------------------------+

Zgodnie z konwencją, metoda ``getName()`` w klasie pakietu powinna zwracać nazwę
klasy.

.. note::

    Jeśli planuje się udostępnić pakiet publicznie, należy użyć nazwy klasy
    pakietu zawierając w niej nazwę repozytorium (na przykład ``AcmeBlogBundle``
    a nie ``BlogBundle``).

.. note::

    Podstawowe pakiety Symfony2 nie prefiksują klasy pakietu słowem ``Symfony``,
    a dodatkowo zawsze dodają podprzestrzeń nazw ``Bundle``; na przykład:
    :class:`Symfony\\Bundle\\FrameworkBundle\\FrameworkBundle`.

Każdy pakiet ma alias, który jest krótszą nazwą pakietu pisaną małymi literami
z użyciem podkreśleń (na przykład ``acme_hello`` dla ``AcmeHelloBundle``, albo
``acme_social_blog`` dla ``Acme\Social\BlogBundle``). Ten alias jest używany
do wymuszenia unikatowości wewnątrz pakietu (poniżej kilka przykładów)

Struktura katalogów
-------------------

Podstawowa struktura katalogów pakietu ``HelloBundle`` przedstawia się następująco:

.. code-block:: text

    XXX/...
        HelloBundle/
            HelloBundle.php
            Controller/
            Resources/
                meta/
                    LICENSE
                config/
                doc/
                    index.rst
                translations/
                views/
                public/
            Tests/

Katalog(i) ``XXX`` odzwierciedlają strukturę przestrzeni nazw w pakiecie.

Poniższe pliki są obowiązkowe:

* ``HelloBundle.php``;
* ``Resources/meta/LICENSE``: Pełna licencja na kod;
* ``Resources/doc/index.rst``: Plik główny dla dokumentacji Pakietu.

.. note::

    Konwencje te zapewniają, że zautomatyzowane narzędzie mogą polegać na
    tej domyślnej strukturze przy wykonywaniu swojej pracy.

Głębokośc podkatalogów należy ograniczyć do mimimum dla najczęściej stosowanych
klas i plików (2 poziomu to maksimum). Więcej poziomów można definiować
dla niestrategicznych, rzadziej używanych plików.

Katalog pakietu jest tylko do odczytu. Jeśli zachodzi potrzeba zapisu w nim
plików tymczasowych, powinno się je przechowywać w katalogach ``cache/`` albo
``log/`` w rozwijanej aplikacji. Narzędzia mogą generować pliki w strukturze
katalogów pakietu tylko wtedy, gdy tworzone pliki będą częścią repozytorium.

Poniższe klasy i pliki mają swoje określone lokalizacje:

+---------------------------------+-----------------------------+
| Typ                             | Katalog                     |
+=================================+=============================+
| Komendy                         | ``Command/``                |
+---------------------------------+-----------------------------+
| Kontrolery                      | ``Controller/``             |
+---------------------------------+-----------------------------+
| Rozszerzenia kontenera usług    | ``DependencyInjection/``    |
+---------------------------------+-----------------------------+
| Detektor zdarzeń                | ``EventListener/``          |
+---------------------------------+-----------------------------+
| Konfiguracja                    | ``Resources/config/``       |
+---------------------------------+-----------------------------+
| Zasoby publiczne                | ``Resources/public/``       |
+---------------------------------+-----------------------------+
| Pliki tłumaczeń                 | ``Resources/translations/`` |
+---------------------------------+-----------------------------+
| Szablony                        | ``Resources/views/``        |
+---------------------------------+-----------------------------+
| Testy jednostkowe i funkcjonalne| ``Tests/``                  |
+---------------------------------+-----------------------------+

.. note::

    Budując pakiet wielokrotnego użytku, klasy modelu powinny być umieszczone
    w przestrzeni nazw ``Model``. Zobacz :doc:`/cookbook/doctrine/mapping_model_classes`
    aby dowiedzieć się jak obsługiwać mapowanie by przechodziły proces kompilacji.

Klasy
-----

Struktura katalogów pakietu jest używana do budowania hierarchii przestrzeni
nazw. Na przykład kontroler ``HelloController`` jest przechowywany w
``Bundle/HelloBundle/Controller/HelloController.php``, zaś pełna nazwa klasy
to ``Bundle\HelloBundle\Controller\HelloController``.

Wszystkie klasy i pliki muszą przestrzegać :doc:`standardów</contributing/code/standards>`
kodowania Symfony2.

Niektóre klasy powinny pełnić rolę fasad i być tak zwięzłe jak to możliwe, tak jak
Commands, Helpers, Listeners, i Controllers.

Klasy łączące się z Dyspozytorem Zdarzeń powinny posiadać przyrostek ``Listener``.

Klasy wyjątków powinny być przechowywane w podprzestrzeni ``Exception``.

Dostawcy
--------

Pakiet nie może osadzać zewnętrznych blibliotek PHP. Zamiast tego, powinien
on polegać na standardowym mechanizmie autoloadingu w Symfony2.

Pakiet nie powinien również dodawać zewnętrznych bilbliotek napisanych w JavaScript,
CSS, lub każdym innym języku.

Testy
-----

Pakiet powinien zawierać w sobie zestaw testów w PHPUnit przechowywanych
w katalogu ``Tests/``. Testy powinny przestrzegać następujących zasad:

* Zestaw testów musi być wykonywalny z użyciem prostej komendy ``phpunit``
  wywoływanej w przykładowej aplikacji;
* Testy funkcjonalne powinny być używane tylko do testowania rezultatów
  odpowiedzi serwera, ewentualnie do zbierania informacji o profilowaniu, o
  ile miało to miejsce;
* Testy powinny pokrywać przynajmniej 95% podstawowego kodu.

.. note::
   Zestaw testów nie może zawierać skryptów ``AllTests.php``, ale musi opierać
   się na istnieniu pliku ``phpunit.xml.dist``.

Dokumentacja
------------

Wszystkie klasy i funkcje muszą być w pełni udokumentowane w PHPDoc.

Obszerna dokumentacja powinna być trzymana w formacie :doc:`reStructuredText
</contributing/documentation/format>` w katalogu ``Resources/doc/``
; plik ``Resources/doc/index.rst`` jest jedynym, obowiązkowym plikiem i
musi być punktem wyjścia dla całej dokumentacji.

Kontrolery
----------

Zgodnie z zaleceniami, kontrolory w pakiecie, które będą dystrybuowane dla
innych nie mogą rozszerzać klasy bazowej :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`.
Zamiast tego, mogą implementować :class:`Symfony\\Component\\DependencyInjection\\ContainerAwareInterface`
albo rozszerzać :class:`Symfony\\Component\\DependencyInjection\\ContainerAware`.

.. note::

    Gdyby spojrzeć na metody kontrolera :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`,
    widać, że są to w głównej mierze skróty ułatwiające naukę.

Trasowanie (routing)
--------------------

Jeśli pakiet dostarcza trasy, muszą one zostać poprzedzone aliasem pakietu.
Na przykład dla AcmeBlogBundle, wszystkie trasy powinny zawierać prefiks
``acme_blog_``.

Szablony
--------

Jeśli pakiet dostarcza szablonów, muszą one korzystać z systemu szablonów Twig.
Pakiet nie może dostarczać głównego układu, chyba że dostarcza w pełni działającą
aplikację.

Pliki tłumaczeń
---------------

Jeśli pakiet zawiera tłumaczenia komunikatów, muszą być one zdefiniowane w
formacie XLIFF; domeny powinny być nazwane po nazwie pakietu. (``bundle.hello``).

Pakiet nie może nadpisywać istniejącego komunikatu z innego pakietu.

Konfiguracja
------------

Aby zapewnić większą elastyczność, pakiet może dostarczyć konfigurowalnych
ustawień przy użyciu wbudowanych mechanizmów Symfony2.

Dla prostych ustawień można polegać na domyślnym wpisie ``parameters`` w
konfiguracji Symfony2. Parametry w Symfony2 to prosta para klucz/wartość;
wartość jest dowolną, prawidłową wartością PHP. Każda nazwa parametru powinna
zaczynać się od aliasu pakietu, choć jest to tylko zalecane praktyka. Reszta
nazwy parametru będzie używać kropki (``.``) w celu oddzielenia różnych części
(na przykład ``acme_hello.email.from``).

Użytkownik może wprowadzić wartości w dowolnym pliku konfiguracyjnym:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        parameters:
            acme_hello.email.from: fabien@example.com

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="acme_hello.email.from">fabien@example.com</parameter>
        </parameters>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->setParameter('acme_hello.email.from', 'fabien@example.com');

    .. code-block:: ini
       :linenos:

        ; app/config/config.ini
        [parameters]
        acme_hello.email.from = fabien@example.com

Pobieranie parametrów konfiguracyjnych w kodzie z kontenera::

    $container->getParameter('acme_hello.email.from');

Nawet jeśli ten mechanizm jest prosty, zachęca się do korzystania z semantycznej
konfiguracji opisanej w Receptariuszu.

.. note::

    Jeśli definiuje się usługi, powinny one również zostać poprzedzone aliasem
    pakietu.

Dalsza lektura
--------------

* :doc:`/cookbook/bundles/extension`

.. _standardami: http://symfony.com/PSR0
