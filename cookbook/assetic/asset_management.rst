.. index::
   single: Assetic; Wprowadzenie

Jak zastosować Assetic przy zarządzaniu zewnętrznymi zasobami
=======================================

Assetic łączy w sobie dwie idee: :ref:`assets<cookbook-assetic-assets>` i :ref:`filters<cookbook-assetic-filters>`. Zewnętrzne zasoby to pliki CSS, JavaScript i obrazów.
Filtry to coś, co może zostać zaaplikowane do owych zasobów zanim te zostaną przesłane do przeglądarki. Pozwala to na zachowanie separacji pomiędzy plikami zewnętrznych zasobów trzymanymi w aplikacji, 
a plikami rzeczywiście prezentowanymi użytkownikowi.

Bez Assetic, pliki obecne w aplikacji są prezentowane bezpośrednio:

.. configuration-block::

    .. code-block:: html+jinja

        <script src="{{ asset('js/script.js') }}" type="text/javascript" />

    .. code-block:: php

        <script src="<?php echo $view['assets']->getUrl('js/script.js') ?>" type="text/javascript" />

Dzięki Assetic można manipulować zewnętrznymi zasobami na różne sposoby (albo też dołączać je z różnych miejsc) zanim zostaną wysłane do użytkownika. Znaczy to, że można:

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

Jedną z cech Assetic jest łączenie wielu plików w jeden. Pozwala to zredukować ilość zapytań HTTP, co jest pożądane dla wydajności aplikacji. Pozwala to również utrzymywać pliki poprzez dzielenie ich na łatwiejsze w utrzymaniu części.
Sposób ten ułatwia ponowne użycie gdyż z łatwością dzieli specyficzne dla projektu pliki od tych, które mogłyby zostać użyte w innych aplikacjach, wciąż serwując je jako jeden plik:

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

W środowisku ``dev`` każdy plik jest wciąż serwowany indywidualnie, dzięki czemu można całkiem łatwo okiełznać problem. Natomiast środowisko ``prod`` (bardziej szczegółowo, gdy flaga ``debug`` jest ustawiona na ``false``), wyrenderuje wszystko jako jeden ``script``, na którego zawartość będą składać się wszystkie użyte pliki JavaScript.

.. tip::

    If you're new to Assetic and try to use your application in the ``prod``
    environment (by using the ``app.php`` controller), you'll likely see
    that all of your CSS and JS breaks. Don't worry! This is on purpose.
    For details on using Assetic in the ``prod`` environment, see :ref:`cookbook-assetic-dumping`.

And combining files doesn't only apply to *your* files. You can also use Assetic to
combine third party assets, such as jQuery, with your own into a single file:

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

Once they're managed by Assetic, you can apply filters to your assets before
they are served. This includes filters that compress the output of your assets
for smaller file sizes (and better front-end optimization). Other filters
can compile JavaScript file from CoffeeScript files and process SASS into CSS.
In fact, Assetic has a long list of available filters.

Many of the filters do not do the work directly, but use existing third-party
libraries to do the heavy-lifting. This means that you'll often need to install
a third-party library to use a filter.  The great advantage of using Assetic
to invoke these libraries (as opposed to using them directly) is that instead
of having to run them manually after you work on the files, Assetic will
take care of this for you and remove this step altogether from your development
and deployment processes.

To use a filter, you first need to specify it in the Assetic configuration.
Adding a filter here doesn't mean it's being used - it just means that it's
available to use (you'll use the filter below).

For example to use the JavaScript YUI Compressor the following config should
be added:

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

Now, to actually *use* the filter on a group of JavaScript files, add it
into your template:

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

A more detailed guide about configuring and using Assetic filters as well as
details of Assetic's debug mode can be found in :doc:`/cookbook/assetic/yuicompressor`.

Kontrolowanie używanych URL
------------------------

If you wish to, you can control the URLs that Assetic produces. This is
done from the template and is relative to the public document root:

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

    Symfony also contains a method for cache *busting*, where the final URL
    generated by Assetic contains a query parameter that can be incremented
    via configuration on each deployment. For more information, see the
    :ref:`ref-framework-assets-version` configuration option.

.. _cookbook-assetic-dumping:

Zrzut plików zewnętrznych zasobów
-------------------

In the ``dev`` environment, Assetic generates paths to CSS and JavaScript
files that don't physically exist on your computer. But they render nonetheless
because an internal Symfony controller opens the files and serves back the
content (after running any filters).

This kind of dynamic serving of processed assets is great because it means
that you can immediately see the new state of any asset files you change.
It's also bad, because it can be quite slow. If you're using a lot of filters,
it might be downright frustrating.

Fortunately, Assetic provides a way to dump your assets to real files, instead
of being generated dynamically.

Dumping Asset Files in the ``prod`` environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the ``prod`` environment, your JS and CSS files are represented by a single
tag each. In other words, instead of seeing each JavaScript file you're including
in your source, you'll likely just see something like this:

.. code-block:: html

    <script src="/app_dev.php/js/abcd123.js"></script>

Moreover, that file does **not** actually exist, nor is it dynamically rendered
by Symfony (as the asset files are in the ``dev`` environment). This is on
purpose - letting Symfony generate these files dynamically in a production
environment is just too slow.

Instead, each time you use your app in the ``prod`` environment (and therefore,
each time you deploy), you should run the following task:

.. code-block:: bash

    $ php app/console assetic:dump --env=prod --no-debug

This will physically generate and write each file that you need (e.g. ``/js/abcd123.js``).
If you update any of your assets, you'll need to run this again to regenerate
the file.

Dumping Asset Files in the ``dev`` environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

By default, each asset path generated in the ``dev`` environment is handled
dynamically by Symfony. This has no disadvantage (you can see your changes
immediately), except that assets can load noticeably slow. If you feel like
your assets are loading too slowly, follow this guide.

First, tell Symfony to stop trying to process these files dynamically. Make
the following change in your ``config_dev.yml`` file:

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

Next, since Symfony is no longer generating these assets for you, you'll
need to dump them manually. To do so, run the following:

.. code-block:: bash

    $ php app/console assetic:dump

This physically writes all of the asset files you need for your ``dev``
environment. The big disadvantage is that you need to run this each time
you update an asset. Fortunately, by passing the ``--watch`` option, the
command will automatically regenerate assets *as they change*:

.. code-block:: bash

    $ php app/console assetic:dump --watch

Since running this command in the ``dev`` environment may generate a bunch
of files, it's usually a good idea to point your generated assets files to
some isolated directory (e.g. ``/js/compiled``), to keep things organized:

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

