.. index::
   single: Assetic; Wprowadzenie

Jak zastosować Assetic do zarządzania aktywami
=======================================

Assetic łączy w sobie dwie idee: :ref:`aktywa<cookbook-assetic-assets>` i :ref:`filtry<cookbook-assetic-filters>`. Aktywa to pliki CSS, JavaScript i obrazów.
Filtry to coś, co może zostać użyte na aktywach zanim te zostaną przesłane do przeglądarki. Pozwala to na zachowanie separacji pomiędzy plikami aktywów trzymanymi w aplikacji, 
a plikami rzeczywiście prezentowanymi użytkownikowi.

Bez Assetic, pliki obecne w aplikacji są serwowane bezpośrednio:

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>" type="text/javascript" />

Dzięki Assetic można manipulować aktywami na różne sposoby (albo też dołączać je z różnych miejsc) zanim zostaną wysłane do użytkownika. Znaczy to, że można:

* Skompresować i połączyć wszystkie pliki CSS i JS w jeden

* Przepuścić wszystkie (albo tylko część) pliki CSS i JS przez pewien rodzaj kompilatora, taki jak LESS, SASS albo CoffeeScript

* Zastosować optymalizacje obrazu na plikach obrazów

.. _cookbook-assetic-assets:

Zasoby zewnętrzne
------

Korzystanie z Assetic przynosi wiele zalet nad metodą bezpośredniego obsługiwania plików. Zasoby nie muszą być trzymane w miejscu z którego są serwowane i mogą zostać wyciągniete z różnych źródeł takich jak bundle.

Można używać Assetic zarówno dla :ref:`CSS stylesheets<cookbook-assetic-including-css>` jak i :ref:`JavaScript files<cookbook-assetic-including-javascript>`. Filozofia dodawania obydwóch jest identyczna, z nieznacznie zmienioną składnią.

.. _cookbook-assetic-including-javascript:

Dołączanie plików JavaScript
~~~~~~~~~~~~~~~~~~~~~~~~~~

By dołączyć pliki JavaScript, można w szablonie zastosować etykietę ``javascript``. Etykieta ta najczęściej spotykana jest w bloku ``javascripts``, jeśli operujesz domyślnie na Standardowej Dystrybucji Symfony, nazwy bloków wyglądają następująco:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' %}
            <script type="text/javascript" src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*')
        ) as $url): ?>
            <script type="text/javascript" src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. tip::

    Można również dołączyć style CSS: zobacz :ref:`cookbook-assetic-including-css`.

W tym przykładzie, wszystkie pliki w katalogu ``Resources/public/js/`` z ``AcmeFooBundle`` zostaną wczytane i zaserwowane z różnych miejsc. Rzeczywista etykieta mogłaby wyglądać na przykład tak:
    
.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

Kluczowym punktem jest: gdy pozwolisz Assetic obsługiwać zewnętrzne zasoby, pliki te zostaną dostarczane z róznych miejsc. To *będzie* powodowało problemy z plikami CSS, które odwołują się do obrazów poprzez ścieżki względne. Zobacz :ref:`cookbook-assetic-cssrewrite`.

.. _cookbook-assetic-including-css:

Dołączanie stylów CSS
~~~~~~~~~~~~~~~~~~~~~~~~~

Aby dostarczyć pliki CSS, można użyć tych samych metodologii co powyżej, za wyjątkiem etykiety, której nazwa powinna być ``stylesheets``. Jeśli domyślnie używasz Standardowej Dystrybucji Symfony, pliki CSS znajdziesz w bloku ``stylesheets``:

.. configuration-block::

    .. code-block:: html+jinja

        {% stylesheets 'bundles/acme_foo/css/*' filter='cssrewrite' %}
            <link rel="stylesheet" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->stylesheets(
            array('bundles/acme_foo/css/*'),
            array('cssrewrite')
        ) as $url): ?>
            <link rel="stylesheet" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

Z uwagi na to, że Assetic zmienia ścieżki do zewnętrznych zasobów, zainfekuje to obrazy tła (lub inne zasoby), które używają ścieżek względnych, chyba, że zostanie zastosowany filtr :ref:`cssrewrite<cookbook-assetic-cssrewrite>`.

.. note::

    Zauważ, że w przykładzie, w którym dołączałeś pliki JavaScript, odnosiłeś się do nich z użyciem ``@AcmeFooBundle/Resources/public/file.js``, jednak teraz odwołujesz się do plików CSS używając rzeczywistej, publicznie widocznej ścieżki: ``bundles/acme_foo/css``. Możesz używać obu metod, wiedz tylko, że istnieje znany problem, który powoduje niepowodzenie filtra ``cssrewrite`` przy użyciu składni ``@AcmeFooBundle``.

.. _cookbook-assetic-cssrewrite:

Ustalanie ścieżki w plikach CSS z użyciem filtra ``cssrewrite``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ponieważ Assetic generuje nowe URL'e dla Twoich zewnętrznych zasobów, jakakolwiek ścieżka względna wewnątrz Twoich plików CSS nie będzie działać. By temu zaradzić, upewnij się, że używasz filtra ``cssrewrite`` w etykiecie ``stylesheets``. Pozwala to przeanalizować Twoje pliki CSS i poprawić wewnętrznie wszystkie ścieżki by odzwierciedlały nową lokalizację.

Możesz prześledzić przykład z poprzedniej sekcji.

.. caution::

  Gdy używasz filtru ``cssrewrite``, nie odwołuj się do plików CSS z użyciem składni ``@AcmeFooBundle``. Aby poznać szczegóły, prześledź notę z sekcji powyżej.

Łączenie zasobów
~~~~~~~~~~~~~~~~

Jedną z cech Assetic jest łączenie wielu plików w jeden. Pomaga to zredukować ilość zapytań HTTP, co jest niezbędne dla wydajności aplikacji. Umożliwia to również sprawniejsze zarządanie plikami poprzez dzielenie ich na mniejsze, łatwiejsze w utrzymaniu części. Wpływa to na reużywalność, bowiem pozwala oddzielić pliki specyficzne dla danego projektu od tych, które mogą zostać użyte w innych aplikacjach, wciąż prezentując je jako jeden zasób:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/*'
            '@AcmeBarBundle/Resources/public/js/form.js'
            '@AcmeBarBundle/Resources/public/js/calendar.js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/*',
                '@AcmeBarBundle/Resources/public/js/form.js',
                '@AcmeBarBundle/Resources/public/js/calendar.js',
            )
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

W środowisku ``dev`` każdy plik jest serwowany indywidualnie, dzięki czemu można okiełznać problem. Natomiast środowisko ``prod`` (mówiąc szczegółowiej, gdy flaga ``debug`` jest ustawiona na ``false``), wyrenderuje wszystko w jednym znaczniku ``script``, który skumuluje zawartość wszystkich użytych przez Ciebie plików JavaScript.

.. tip::

    Jeśli dopiero co poznajesz Assetic i uruchamiasz aplikacje w środowisku ``prod`` (poprzez użycie kontrolera ``app.php``), prawdopodobnie doświadczysz, że wszystkie Twoje pliki CSS i JS uległy zniszczeniu. Nie przejmuj się! Jest to celowe zachowanie. Po szczegółowe informacje na temat używania Assetic w środowisku ``prod`` sięgnij do :ref:`cookbook-assetic-dumping`.

Łączenie plików nie odnosi się tylko i wyłącznie do *swoich* plików. Można równie dobrze użyć Assetic w celu połączenia zasobów zewnętrznych, takich jak jQuery, z własnymi i zawrzeć je w pojedynczym pliku:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts
            '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js'
            '@AcmeFooBundle/Resources/public/js/*' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/thirdparty/jquery.js',
                '@AcmeFooBundle/Resources/public/js/*',
            )
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. _cookbook-assetic-filters:

Filtry
-------

Gdy są one zarządzane przez Assetic, można zastosować filtry do zasobów zanim te zostaną zaserwowane użytkownikowi. Wliczając w nie filtry, które kompresują rezultat użytych zasobów do mniejszych rozmiarów (i optymalizują aplikację), jak i te, które mogą skompilować plik JavaScript z plików CoffeeScript albo przetworzyć SASS w CSS. W rzeczywistości, Assetic ma dość pokaźną listę dostępnych filtrów.

Wiele z tych filtrów nie działa bezpośrednio, gdyż używa bibliotek firm trzecich do wykonywania najcięższej, algorytmicznej pracy. Oznacza to, że nieraz będzie trzeba zainstalować biblioteki firm trzecich, by potem zmusić dany filtr do działania. Ogromną zaletą korzystania z Assetic do wywoływania tych bibliotek (w przeciwieństwie do używania ich bezpośrednio) jest to, że zamiast uruchamiać je ręcznie podczas pracy, Assetic zadba o to za nas i usunie ten krok z procesu rozwoju i wdrażania aplikacji.

By skorzystać z filtru, trzeba go najpierw skonfigurować w Assetic. Dodanie filtru tutaj nie znaczy, że jest już używany - oznacza to po prostu, że jest dostępny do wykorzystania (użyjemy filtru poniżej).

Na przykład, by użyć JavaScript YUI Compressor, następująca konfiguracja powinna zostać dodana:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));

Teraz, by tak naprawdę *użyć* filtru na grupie plików JavaScript, wystarczy dodać poniższe linie do szablonu:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='yui_js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

Bardziej szczegółowy przewodnik na temat konfiguracji i korzystania z filtrów Assetic, jak również informacji o trybie debugowania Assetic można znaleźć w :doc:`/cookbook/assetic/yuicompressor`.

Kontrolowanie używanych adresów URL
------------------------

Jeśli chcesz, możesz kontrolować adresy URL generowane przez Assetic. Są one tworzone z szablonu i relatywne do głównego dokumentu publicznego:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' output='js/compiled/main.js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    Symfony zawiera metody do *niszczenia* cache, gdzie finalny adres URL generowany przez Assetic zawiera parametr zapytania, który może być zwiększany w konfiguracji przy każdym procesie wdrażania. Aby uzyskać więcej informacji, zobacz na opcje konfiguracji :ref:`ref-framework-assets-version`.

.. _cookbook-assetic-dumping:

Zrzut plików aktywów
-------------------

W środowisku ``dev``, Assetic generuje ścieżki do plików CSS i JavaScript, które nie istnieją fizycznie na komputerze. Ścieżki sa tak czy inaczej generowane, gdyż wewnętrzny kontroler Symfony jest w stanie otworzyć pliki, by zaserwować ich zawartość (zaraz po uruchomieniu filtrów).

Ten rodzaj dynamicznego serwowania przetworzonych aktywów daje dużo korzyści, gdyż oznacza to, że można od razu zobaczyć stan wszystkich plików aktywów, które uległy zmianie. Z drugiej strony, może przynieśc i straty z uwagi na spowolnienie aplikacji. Jeśli używa się zbyt wielu filtrów, może okazać się to wręcz frustrujące.

Na szczęście Assetic zapewnia możliwość zrzutu aktywów do rzeczywistych plików, zamiast generowania ich dynamicznie.


Zrzut plików aktywów w środowisku ``prod``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W środowisku ``prod``, pliki JS i CSS sa reprezentowane przez pojedynczy znacznik. Innymi słowy, zamiast widzieć każdy plik JavaScript, który załączono w źródle, nieraz zobaczymy coś takiego:

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

Co więcej, ten plik tak naprawdę **nie** istnieje, ani też nie jest dynamicznie generowany przez Symfony (jak pliki aktywów w środowisku ``dev``). Jest to celowe - pozwolenie Symfony na generowanie tych plików dynamicznie w środowisku produkcyjnym byłoby po prostu za wolne.

Zamiast tego, za każdym gdy używa się środowiska ``prod`` (a zatem za każdym razem gdy wdrażano), powinno sie uruchomić następujące zadanie:

.. code-block:: bash

    $ php app/console assetic:dump --env=prod --no-debug

To spowoduje fizyczną generację każdego pliku, który potrzeba. (np: ``/js/abcd123.js``). W przypadku aktualizacji aktywów, trzeba uruchomić to zadanie ponownie i przegenerować pliki.

Zrzut plików aktywów w środowisku ``dev``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Domyślnie, każda ścieżka aktywów generowana w środowisku ``dev`` jest obsługiwana dynamicznie przez Symfony. Nie ma to wad (widać zmiany natychmiast), z wyjątkiem, że aktywa mogą ładować się zauważalnie wolniej. Jeśli uważasz, że aktywa wczytują się zbyt wolno, skorzystaj z tej instrukcji.

Po pierwsze, nakaż Symfony aby zatrzymać przetwarzanie tych plików dynamicznie. Zmień konfigurację w pliku ``config_dev.yml`` następująco:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        assetic:
            use_controller: false

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->
        <assetic:config use-controller="false" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('assetic', array(
            'use_controller' => false,
        ));

Następnie, ponieważ Symfony nie jest już odpowiedzialne za generowanie aktywów, trzeba zrzucić je ręcznie. Aby to zrobić, wykonaj następujące czynności:

.. code-block:: bash

    $ php app/console assetic:dump

To zadanie fizycznie zapisuje wszystkie pliki aktywów dla środowiska ``dev``. Dużą wadą jest, że trzeba uruchamiać je za każdym razem gdy zaktualizowano aktyw. Na szczęście, stosując opcje ``--watch``, polecenie automatycznie przegeneruje aktywa *w chwili ich zmiany*:

This physically writes all of the asset files you need for your ``dev``
environment. The big disadvantage is that you need to run this each time
you update an asset. Fortunately, by passing the ``--watch`` option, the
command will automatically regenerate assets *as they change*:

.. code-block:: bash

    $ php app/console assetic:dump --watch

Ponieważ uruchomienie tego polecenia w środowisku ``dev`` może wygererować dość sporo plików, zazwyczaj dobrym pomysłem dla tak generowanych plików aktywów jest wskazanie odizolowanego katalogu (np: ``/js/compiled``), by wciąż przechowywać rzeczy w sposób zorganizowany:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' output='js/compiled/main.js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array(),
            array('output' => 'js/compiled/main.js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

