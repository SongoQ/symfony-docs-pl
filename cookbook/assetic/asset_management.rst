.. index::
   single: Assetic; Wprowadzenie

Jak zastosować Assetic do zarządzania aktywami
=======================================

Assetic łączy w sobie dwie podstawowe idee: :ref:`aktywów<cookbook-assetic-assets>` i :ref:`filtrów<cookbook-assetic-filters>`. Aktywa to pliki CSS, JavaScript i obrazów. Filtry to coś, co może być zastosowane na aktywach zanim te zostaną przesłane do przeglądarki. Pozwala to odseparować pliki aktywów trzymane w aplikacji od plików rzeczywiście prezentowanych użytkownikowi.

Bez Assetic, pliki obecne w aplikacji są serwowane bezpośrednio:

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>" type="text/javascript" />

Jednakże dzięki Assetic można manipulować aktywami na różne sposoby (albo załadować je z dowolnego miejsca) zanim te zostaną wyświetlone użytkownikowi. Oznacza to, że można:

* Skompresować i połączyć wszystkie pliki CSS i JS w jeden

* Przepuścić wszystkie (albo tylko część) pliki CSS i JS przez jakiś kompilator, taki jak LESS, SASS albo CoffeeScript

* Zastosować optymalizacje obrazu na zdjęciach

.. _cookbook-assetic-assets:

Aktywa
------

Korzystanie z Assetic posiada wiele zalet nad metodą bezpośredniego serwowania plików. Pliki nie musząbyć przechowywane w miejscach z których są serwowane oraz mogą zostać pobrane z różnych źródeł, takich jak pakiety.

Można używać Assetic zarówno dla :ref:`stylów CSS<cookbook-assetic-including-css>` jak i :ref:`plików JavaScript<cookbook-assetic-including-javascript>`. Idea przewodnia dodawania obu jest w zasadzie taka sama, ale z nieco inną składnią.

.. _cookbook-assetic-including-javascript:

Dołączanie plików JavaScript
~~~~~~~~~~~~~~~~~~~~~~~~~~

By dołączyć pliki JavaScript, można w szablonie zastosować znacznik ``javascript``. Jest on najczęściej spotykany w bloku ``javascripts``, o ile używano domyślnej nazwy bloków ze Standardowej Dystrybucji Symfony:

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

W tym przykładzie wszystkie pliki w katalogu ``Resources/public/js/`` z ``AcmeFooBundle`` zostaną wczytane i zaserwowane z innych lokalizacji. Rzeczywisty znacznik mogłby wyglądać na przykład tak:
    
.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

Punktem kluczowym jest: gdy pozwolisz Assetic obsługiwać swoje aktywa, będą one serwowane z róznych lokalizacji. *Będzie* to powodować problemy z plikami CSS, które odwołują się do obrazów poprzez ścieżki względne. Zobacz :ref:`cookbook-assetic-cssrewrite`.

.. _cookbook-assetic-including-css:

Dołączanie stylów CSS
~~~~~~~~~~~~~~~~~~~~~~~~~

Aby dostarczyć pliki CSS, można użyć tych samych metod co powyżej, z wyjątkiem znacznika ``stylesheets``. Jeśli domyślnie używano Standardowej Dystrybucji Symfony, pliki CSS powinny znajdować się w bloku ``stylesheets``:

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

Z uwagi na to, że Assetic zmienia ścieżki do swoich aktywów, najprawdopodobniej obrazy tła przestaną działać (lub inne zasoby), które używają ścieżek względnych, chyba, że zastosowano filtr :ref:`cssrewrite<cookbook-assetic-cssrewrite>`.

.. note::

    Zauważ, że w pierwotnym przykładzie, gdzie dołączano pliki JavaScript, odnoszono się do nich z użyciem ``@AcmeFooBundle/Resources/public/file.js``, zaś w tym przykładzie odwołanie do plików CSS następuje poprzez rzeczywistą, publicznie widoczną ścieżkę: ``bundles/acme_foo/css``. Można używać obu metod, należy jednak pamiętać, że istnieje znany problem, który powoduje błędne działanie filtra ``cssrewrite`` z użyciem składni ``@AcmeFooBundle``.

.. _cookbook-assetic-cssrewrite:

Ustalanie ścieżki w plikach CSS z użyciem filtra ``cssrewrite``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ponieważ Assetic generuje nowe URL'e dla Twoich aktywów, wszystkie ścieżki względne wewnątrz plików CSS nie będa działać. By temu zaradzić, upewnij się, że użyto filtra ``cssrewrite`` w znaczniku ``stylesheets``. Pozwala on przeanalizować pliki CSS i skorygować wszystkie ścieżki wewnętrznie tak, by odzwierciedlały nową położenie.

Można zobaczyć przykład z poprzedniej części.

.. caution::

  Przy stosowaniu filtra ``cssrewrite``, nie powinno się odnosić do plików CSS za pomocą składni ``@AcmeFooBundle``. Zobacz wiadomości z poprzedniej części, aby poznać szczegóły.

Łączenie aktywów
~~~~~~~~~~~~~~~~

Jedną z cech Assetic jest łączenie wielu plików w jeden. Pomaga to zredukować liczbę żądań HTTP, co jest niezbędne dla wydajności części publicznej aplikacji. Pozwala to także na sprawniejsze zarządanie plikami poprzez dzielenie ich na mniejsze, łatwiejsze w utrzymaniu części. Wpływa to na wieloużywalność, bowiem pozwala oddzielić pliki specyficzne dla danego projektu od tych, które mogą zostać użyte w innych aplikacjach, wciąż serwując je jako jeden plik:

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

W środowisku ``dev`` każdy plik jest nadal serwowany indywidualnie, tak aby można było debugować problemy łatwiej. Jednak w środowisku ``prod`` (a dokładniej, gdy flaga ``debug`` jest ustawiona na ``false``), wszystko zostanie wygenerowane w jednym znaczniku ``script``, który zawierał będzie zawartość wszystkich użytych plików JavaScript.

.. tip::

    Jeśli dopiero co poznajesz Assetic i uruchamiasz aplikacje w środowisku ``prod`` (za pomocą kontrolera ``app.php``), prawdopodobnie doświadczysz, że wszystkie pliki CSS i JS przestały działać. Nie martw się! Jest to celowe. Po szczegółowe informacje dotyczące korzystania Assetic w środowisku ``prod`` sięgnij do :ref:`cookbook-assetic-dumping`.

Łączenie plików odnosi się nie tylko do *swoich* plików. Można również użyć Assetic do połączenia zasobów osób trzecich, takich jak jQuery, z własnymi i połączyć je w jednym pliku:

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

Gdy są one zarządzane przez Assetic, można zastosować filtry do aktywów, zanim te zostaną zaserwowane użytkownikowi. Obejmuje to filtry, które kompresują dane wyjściowe aktywów do mniejszych rozmiarów (i poprawiają wydajność części publicznej aplikacji). Inne filtry mogą skompilować plik JavaScript z plików CoffeeScript albo przetworzyć SASS w CSS. W rzeczywistości, Assetic ma dość pokaźną listę dostępnych filtrów.

Wiele z tych filtrów nie zadziała bezpośrednio, gdyż używa bibliotek firm trzecich do wykonywania najcięższej, algorytmicznej pracy. Oznacza to, że nieraz będzie trzeba zainstalować biblioteki firm trzecich, by potem użyć konkretnego filtru. Zaletą korzystania z Assetic do wywoływania tych bibliotek (w przeciwieństwie do korzystania z nich bezpośrednio) jest to, że zamiast uruchamiać je ręcznie podczas pracy, Assetic zadba o to za nas i usunie ten krok z procesu rozwoju i wdrażania aplikacji.

Aby użyć filtru, trzeba najpierw określić go w konfiguracji Assetic. Dodawanie filtru tutaj nie znaczy, że jest już używany - to po prostu oznacza, że jest możliwy do wykorzystania (można skorzystać z filtra poniżej).

Na przykład, aby użyć JavaScript YUI Compressor, powinna zostać dodana następująca konfiguracja:

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

Teraz, aby rzeczywiście *użyć* filtru na grupie plików JavaScript, wystarczy następująco zmodyfikować plik szablonu:

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

Jeśli chcesz, możesz kontrolować adresy URL generowane przez Assetic. Są one tworzone z szablonu i relatywne w stosunku do głównego dokumentu publicznego:

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

    Symfony zawiera metody do *czyszczenia* pamięci podręcznej, gdzie ostateczny adres URL generowany przez Assetic zawiera parametr zapytania, który może być zwiększany w konfiguracji przy każdym wdrożeniu. Aby uzyskać więcej informacji, zapoznaj się z opcja konfiguracji :ref:`ref-framework-assets-version`.

.. _cookbook-assetic-dumping:

Zrzut plików aktywów
-------------------

W środowisku ``dev``, Assetic generuje ścieżki do plików CSS i JavaScript, które fizycznie nie istnieją na komputerze. Ścieżki są tak czy inaczej generowane, gdyż wewnętrzny kontroler Symfony jest w stanie otworzyć pliki, by zaserwować ich zawartość (zaraz po uruchomieniu filtrów).

Ten rodzaj dynamicznego serwowania przetworzonych aktywów daje dużo korzyści, gdyż oznacza to, że można od razu zobaczyć stan wszystkich plików aktywów, które uległy zmianie. Z drugiej strony, może przynieśc i straty z uwagi na spowolnienie aplikacji. Jeśli używa się zbyt wielu filtrów, może okazać się to wręcz frustrujące.

Na szczęście Assetic zapewnia możliwość zrzutu aktywów do rzeczywistych plików, zamiast generowania ich dynamicznie.


Zrzut plików aktywów w środowisku ``prod``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W środowisku ``prod``, pliki JS i CSS sa reprezentowane przez pojedynczy znacznik. Innymi słowy, zamiast widzieć każdy plik JavaScript, który załączono w źródle, nieraz zobaczy się coś takiego:

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

Co więcej, plik ten w rzeczywistości **nie** istnieje, ani nie jest również dynamicznie generowany przez Symfony (jak pliki aktywów w środowisku ``dev``). Jest to celowe - pozwolenie Symfony na generowanie tych plików dynamicznie w środowisku produkcyjnym byłoby po prostu zbyt wolne.

Zamiast tego, za każdym razem gdy korzysta się ze środowiska ``prod`` (a zatem za każdym razem gdy następuje proces wdrażania), powinno sie uruchomiać następujące zadanie:

.. code-block:: bash

    $ php app/console assetic:dump --env=prod --no-debug

To spowoduje fizyczną generację każdego pliku, którego potrzeba. (np. ``/js/abcd123.js``). W przypadku aktualizacji aktywów, trzeba uruchomić to zadanie ponownie i przegenerować pliki.

Zrzut plików aktywów w środowisku ``dev``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Domyślnie, każda ścieżka aktywa generowana w środowisku ``dev`` jest obsługiwana dynamicznie przez Symfony. Nie ma to wad (zmiany widać natychmiast), z zastrzeżeniem, że aktywa mogą ładować się zauważalnie wolniej. Jeśli uważasz, że aktywa wczytują się zbyt wolno, skorzystaj z tej instrukcji.

Po pierwsze, powiedz Symfony aby zatrzymać przetwarzanie tych plików dynamicznie. Wprowadź następującą zmianę w pliku konfiguracji ``config_dev.yml``:

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

To polecenie fizycznie zapisuje wszystkie pliki aktywów w środowisku ``dev``. Dużą wadą jest, że trzeba uruchamiać je za każdym razem gdy zaktualizowano aktywa. Na szczęście, przekazując opcje ``--watch`` umożliwi się automatycznie ich przegenerowywanie  *w chwili ich zmiany*:

.. code-block:: bash

    $ php app/console assetic:dump --watch

Ponieważ uruchomienie tego polecenia w środowisku ``dev`` może wygererować dość sporo plików, zazwyczaj dobrym pomysłem dla tak generowanych plików jest wskazanie dla nich odizolowanego katalogu (np. ``/js/compiled``), tak aby utrzymać wszystko w sposób zorganizowany:

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

