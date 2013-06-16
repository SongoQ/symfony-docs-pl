.. index::
   single: Assetic; stosowanie filtrów

Jak zastosować filtry w Assetic do określonych rozszerzeń plików
================================================================

Filtry Assetic mogą być stosowane do poszczególnych plików, grup plików a nawet,
jak zobaczysz tutaj, do plików, które mają określone rozszerzenie. Aby pokazać,
jak radzić sobie z każdą opcją, załóżmy, że chcemy używać filtra CoffeeScript,
który kompiluje pliki CoffeeScript do JavaScript.

Główna konfiguracja polega na ustanowieniu ścieżek do coffee i node. Domyślnie są
one ustawione odpowiednio na ``/usr/bin/coffee`` i ``/usr/bin/node``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node" />
        </assetic:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin'  => '/usr/bin/coffee',
                    'node' => '/usr/bin/node',
                ),
            ),
        ));

Filtrowanie pojedynczego pliku
------------------------------

Można teraz serwować pojedynczy plik CoffeeScript jako JavaScript prosto z szablonu:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee' filter='coffee' %}
            <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php
       :linenos:

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee'),
            array('coffee')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

To wszystko co jest potrzebne by skompilować ten plik CoffeeScript i zaserwować go jako skompilowany skrypt JavaScript.        

Filtrowanie wielu plików
------------------------

Można także połączyć wiele plików CoffeeScript w jeden plik wynikowy:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
            filter='coffee' %}
            <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php
       :linenos:

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/example.coffee',
                '@AcmeFooBundle/Resources/public/js/another.coffee',
            ),
            array('coffee')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

Oba pliki będą teraz serwowane jako jeden plik, skompilowany do zwykłego JavaScript.

.. _cookbook-assetic-apply-to:

Filtrowanie na podstawie rozszerzeń plików
------------------------------------------

Jedną z największych zalet korzystania z Assetic jest redukowanie liczby plików
aktywów w celu obniżenia liczby żądań HTTP. Aby w pełni z tego skorzytać, byłoby
dobrze połączyć *wszystkie* pliki JavaScript i CoffeeScript razem, ponieważ będą
one wszystkie ostatecznie zaserwowane jako zwykły JavaScript. Niestety dodanie
plików JavaScript w ten sposób nie zadziała, gdyż zwykłe pliki JavaScript nie
przetrwają kompilacji CoffeeScript.

Można tego uniknąć korzystając w konfiguracji z opcji ``apply_to``, która pozwala
określić, że dany filtr powinien zawsze być stosowany do poszczególnych rozszerzeń
plików. W tym przypadku można określić, że filtr Coffee zostanie zastosowany do
wszystkich plików ``.coffee``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node
                    apply_to: "\.coffee$"

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node"
                apply_to="\.coffee$" />
        </assetic:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin'      => '/usr/bin/coffee',
                    'node'     => '/usr/bin/node',
                    'apply_to' => '\.coffee$',
                ),
            ),
        ));

Dzięki temu nie ma już potrzeby, aby określać filtr ``coffee`` w szablonie.
Można również stosować zwykłe pliki JavaScript, które zostaną połączone i wyrenderowane
jako pojedynczy plik JavaScript (tylko pliki ``.coffee`` zostaną poddane filtrowi CoffeeScript):

.. configuration-block::

    .. code-block:: html+jinja
       :linenos;

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
                       '@AcmeFooBundle/Resources/public/js/regular.js' %}
            <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php
       :linenos:

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/example.coffee',
                '@AcmeFooBundle/Resources/public/js/another.coffee',
                '@AcmeFooBundle/Resources/public/js/regular.js',
            )
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

