.. index::
   single: Assetic; Stosowanie filtrów

Jak zastosować filtry w Assetic do określonych rozszerzeń plików
===========================================================

Filtry Assetic mogą być stosowane do poszczególnych plików, grup plików a nawet, jak zobaczysz tutaj, do plików, które mają określone rozszerzenie. Aby pokazać, jak obsłużyć każdą opcję, załóżmy, że chcemy używać filtra CoffeeScript, który kompiluje pliki CoffeeScript w JavaScript.

Głowna konfiguracja to ustanowienie ścieżek do coffee i node. Domyślnie ustawione są one odpowiednio na ``/usr/bin/coffee`` i ``/usr/bin/node``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node" />
        </assetic:config>

    .. code-block:: php

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
--------------------

Można teraz serwować pojedynczy plik CoffeeScript jako JavaScript prosto z szablonu:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee' filter='coffee' %}
            <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee'),
            array('coffee')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

To wszystko co jest potrzebne by skompilować ten plik CoffeeScript i zaserwować go jako skompilowany JavaScript.        

Filtrowanie wielu plików
---------------------

Można także połączyć wiele plików CoffeeScript w jeden wynikowy plik:

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
            filter='coffee' %}
            <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/example.coffee',
                '@AcmeFooBundle/Resources/public/js/another.coffee',
            ),
            array('coffee')
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

Oba pliki będą teraz serwowane jako jeden plik skompilowane do regularnej postaci JavaScript.

.. _cookbook-assetic-apply-to:

Filtrowanie na podstawie rozszerzenia pliku
-----------------------------------

Jedną z największych zalet korzystania z Assetic jest zredukowanie liczby plików aktywów by obniżyć ilość żądań HTTP. Aby w pełni z tego skorzytać, byłoby dobrze połączyć *wszystkie* pliki JavaScript i CoffeeScript razem, ponieważ będą one ostatecznie wszystkie zaserwowane jako JavaScript. Niestety dodanie plików JavaScript do łączonych w ten sposób plików nie zadziała, gdyż regularne pliki JavaScript nie przetrwają kompilacji CoffeeScript.

Można tego uniknąć korzystając w konfiguracji z opcji ``apply_to``, która pozwala określić, że dany filtr powinien zawsze być stosowany do szczególnych rozszerzeń plików. W tym przypadku można określić, że filtry Coffee zostanie zastosowany do wszystkich plików ``.coffee``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node
                    apply_to: "\.coffee$"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node"
                apply_to="\.coffee$" />
        </assetic:config>

    .. code-block:: php

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

Dzięki temu nie ma już potrzeby, aby określać filtr ``coffee`` w szablonie. Można także wymienić regularne pliki JavaScript, które zostaną połączone i wyrenderowane jako pojedynczy plik JavaScript (tylko pliki ``.coffee`` zostaną przepuszczone przez filtr CoffeeScript):

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
                       '@AcmeFooBundle/Resources/public/js/regular.js' %}
            <script src="{{ asset_url }}" type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array(
                '@AcmeFooBundle/Resources/public/js/example.coffee',
                '@AcmeFooBundle/Resources/public/js/another.coffee',
                '@AcmeFooBundle/Resources/public/js/regular.js',
            )
        ) as $url): ?>
            <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

