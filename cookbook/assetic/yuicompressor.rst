.. index::
   single: Assetic; YUI Compressor

Jak kompresować skrypty i style z użyciem YUI Compressor
========================================================

Yahoo! dostarcza doskonałe narzędzie do minimalizacji plików JavaScript i arkuszy stylów,
dzięki czemu podróżują one przez sieć o wiele szybciej, `YUI Compressor`_.
Dzięki Assetic, można skorzystać z tego narzędzia bardzo łatwo. 

Pobranie pliku JAR YUI Compressor
---------------------------------

YUI Compressor jest napisany w Java i dystrybuowany jako JAR.
`Pobierz JAR`_ ze strony Yahoo! i zapisz go do ``app/Resources/java/yuicompressor.jar``.

Konfiguracja filtrów YUI
------------------------

Pora na skonfigurowanie dwóch filtrów Assetic w aplikacji, jeden do minimalizacji
plików JavaScripts z YUI Compressor i jeden do minimalizacji arkuszy stylów:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            # java: "/usr/bin/java"
            filters:
                yui_css:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"
                yui_js:
                    jar: "%kernel.root_dir%/Resources/java/yuicompressor.jar"

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="yui_css"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
            <assetic:filter
                name="yui_js"
                jar="%kernel.root_dir%/Resources/java/yuicompressor.jar" />
        </assetic:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            // 'java' => '/usr/bin/java',
            'filters' => array(
                'yui_css' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
                'yui_js' => array(
                    'jar' => '%kernel.root_dir%/Resources/java/yuicompressor.jar',
                ),
            ),
        ));
        
.. note::

    Użytkownicy Windows muszą pamiętać o zaktualizowaniu konfiguracji o właściwą
    lokalizację biblioteki java. W Windows7 x64 bitów domyślnie jest to
    ``C:\Program Files (x86)\Java\jre6\bin\java.exe``.

Do dyspozycji są dwa filtry Assetic: ``yui_css`` i ``yui_css``. Będą korzystać z YUI Compressor do minimalizacji arkuszy stylów i plików JavaScript.

Minimalizacja aktywów
---------------------

Pomimo skonfigurowanego YUI Compressor, nic się nie stanie dopóty, dopóki nie
zastosuje się jednego z jego filtrów na używanych aktywach. Ponieważ aktywa są
częścią warstwy widoku, praca ta jest wykonywana w szablonach:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='yui_js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php
       :linenos:

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>

.. note::

    Powyższy przykład zakłada, że posiada się pakiet o nazwie ``AcmeFooBundle``,
    a pliki JavaScripts znajdują się w katalogu ``Resources/public/js``. To nie
    jest tak ważne, gdyż można dołączyć pliki JavaScript bez względu na to gdzie się znajdują.

Po dodaniu filtru ``yui_js`` do znaczników aktywów powyżej, powinno się odczuć,
że tak generowane pliki JavaScript przechodzą przez sieć znacznie szybciej.
Identyczny proces można powtórzyć do zminimalizowania arkuszy stylów.

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:
       
        {% stylesheets '@AcmeFooBundle/Resources/public/css/*' filter='yui_css' %}
            <link rel="stylesheet" type="text/css" media="screen" href="{{ asset_url }}" />
        {% endstylesheets %}

    .. code-block:: html+php
       :linenos:

        <?php foreach ($view['assetic']->stylesheets(
            array('@AcmeFooBundle/Resources/public/css/*'),
            array('yui_css')
        ) as $url): ?>
            <link rel="stylesheet" type="text/css" media="screen" href="<?php echo $view->escape($url) ?>" />
        <?php endforeach; ?>

Wyłączenie minimalizacji w trybie debugowania
---------------------------------------------

Pliki JavaScripts i arkusze stylów po minimalizacji są trudne do odczytania,
nie mówiąc już o samym debugowaniu. Z tego powodu, Assetic pozwala wyłączyć pewien
filtr gdy aplikacja jest w trybie debugowania. Można to zrobić poprzedzając nazwę
filtra w szablonie znakiem zapytania: ``?``. Instruuje on Assetic, by zastosować
ten filtr w momencie, gdy tryb debugowania jest wyłączony.

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {% javascripts '@AcmeFooBundle/Resources/public/js/*' filter='?yui_js' %}
            <script src="{{ asset_url }}"></script>
        {% endjavascripts %}

    .. code-block:: html+php
       :linenos:

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/*'),
            array('?yui_js')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>"></script>
        <?php endforeach; ?>


.. tip::
   
   Zamiast dodawać filtry do znaczników aktywów, można również globalnie włączyć
   je dodając atrybut apply-to do konfiguracji filtra, na przykład w filtrze
   yui_js ``apply_to: "\.js$"``. Aby zastosować to tylko w srodowisku produkcyjnym,
   należy zaktualizować plik konfiguracyjny config_prod zamiast głównego pliku config.
   Szczegółowe informacje na temat stosowania filtrów w zależności od rozszerzenia
   pliku można znaleźć pod adresem :ref:`cookbook-assetic-apply-to`. 


.. _`YUI Compressor`: http://developer.yahoo.com/yui/compressor/
.. _`Pobierz JAR`: http://yuilibrary.com/projects/yuicompressor/
