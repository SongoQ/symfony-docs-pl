.. index::
   single: Assetic; optymalizacja obrazów

Jak zastosować Assetic do optymalizacji obrazów w funkcjach Twiga
=================================================================

Wśród swoich wielu filtrów, Assetic posiada cztery, które mogą być wykorzystane
do optymalizacji obrazów w czasie przetwarzania. Pozwala to uzyskać korzyści
z mniejszych rozmiarów plików bez konieczności używania edytora graficznego
do przetwarzania każdego obrazu z osobna. Wyniki są buforowane i mogą zostać
zrzucone w środowisku produkcyjnym, zatem nie doświadczy się spadku wydajności
dla użytkowników końcowych.

Używanie Jpegoptim
------------------

`Jpegoptim`_ to narzędzie do optymalizacji plików JPEG. Aby korzystać z niego
w Assetic, trzeba dodać poniższe linie kodu do konfiguracji Assetic:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim" />
        </assetic:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                ),
            ),
        ));

.. note::

    Proszę zwrócić uwagę, że aby korzystać z jpegoptim trzeba mieć go zainstalowanego
    w systemie. Opcja ``bin`` wskazuje na lokalizację skompilowanego kodu binarnego.

Można go teraz użyć z szablonu:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {% image '@AcmeFooBundle/Resources/public/images/example.jpg'
            filter='jpegoptim' output='/images/example.jpg' %}
            <img src="{{ asset_url }}" alt="Example"/>
        {% endimage %}

    .. code-block:: html+php
       :linenos:

        <?php foreach ($view['assetic']->images(
            array('@AcmeFooBundle/Resources/public/images/example.jpg'),
            array('jpegoptim')
        ) as $url): ?>
            <img src="<?php echo $view->escape($url) ?>" alt="Example"/>
        <?php endforeach; ?>

Usuwanie wszystkich danych EXIF
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Domyślnie, uruchomienie tego filtru usunie tylko część metainformacji przechowywanych
w pliku. Wszelkie dane EXIF i komentarze nie zostaną usunięte, ale można tego dokonać
używając opcji ``strip_all``:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
                    strip_all: true

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim"
                strip_all="true" />
        </assetic:config>

    .. code-block:: php
       :linenos;

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin'       => 'path/to/jpegoptim',
                    'strip_all' => 'true',
                ),
            ),
        ));

Obniżanie maksymalnej jakości
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Poziom jakości JPEG nie jest domyślnie zmieniany. Można osiągnąć redukcję
rozmiaru pliku poprzez ustawienie maksymalnej jakości obrazu, która będzie niższa
niż obecny poziom. Nastąpi to oczywiście kosztem jakości obrazu: 

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
                    max: 70

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim"
                max="70" />
        </assetic:config>

    .. code-block:: php
       :linenos;

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                    'max' => '70',
                ),
            ),
        ));

Krótsza składnia: funkcja Twig
------------------------------

Jeśli używa się szablonów Twig, to jest możliwe osiągnięcie tego wszystkiego poprzez
zastosowanie krótszej składni i specjalnej funkcji. Aby rozpoczać, należy dodać
następującą konfigurację:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
            twig:
                functions:
                    jpegoptim: ~

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim" />
            <assetic:twig>
                <assetic:twig_function
                    name="jpegoptim" />
            </assetic:twig>
        </assetic:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                ),
            ),
            'twig' => array(
                'functions' => array('jpegoptim'),
                ),
            ),
        ));

Szablon Twig można teraz zmienić następująco:        

.. code-block:: html+jinja

    <img src="{{ jpegoptim('@AcmeFooBundle/Resources/public/images/example.jpg') }}" alt="Example"/>

Można określić katalog docelowy w konfiguracji w następujący sposób:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        assetic:
            filters:
                jpegoptim:
                    bin: path/to/jpegoptim
            twig:
                functions:
                    jpegoptim: { output: images/*.jpg }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="jpegoptim"
                bin="path/to/jpegoptim" />
            <assetic:twig>
                <assetic:twig_function
                    name="jpegoptim"
                    output="images/*.jpg" />
            </assetic:twig>
        </assetic:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'jpegoptim' => array(
                    'bin' => 'path/to/jpegoptim',
                ),
            ),
            'twig' => array(
                'functions' => array(
                    'jpegoptim' => array(
                        output => 'images/*.jpg'
                    ),
                ),
            ),
        ));

.. _`Jpegoptim`: http://www.kokkonen.net/tjko/projects.html
