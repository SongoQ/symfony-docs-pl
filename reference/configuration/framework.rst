.. index::
   single: Configuration Reference; Framework

FrameworkBundle - Konfiguracja ("framework")
============================================

Ten dokument jest nadal w fazie przygotowania. Powinien być dokładny, ale nie 
wszystkie opcje są w pełni opisane.

``FrameworkBundle`` posiada większość z "podstawowych" funkcjonalności frameworka
i może być konfigurowany w konfiguracji aplikacji poprzez klucz ``framework``.
Obejmuje to ustawienia sesji, tłumaczeń, formularzy, walidacji, routingu i wiele więcej.

Konfiguracja
------------

* `charset`_
* `secret`_
* `ide`_
* `test`_
* `form`_
    * :ref:`enabled<config-framework-form-enabled>`
* `csrf_protection`_
    * :ref:`enabled<config-framework-csrf-enabled>`
    * `field_name`

charset
~~~~~~~

**type**: ``string`` **default**: ``UTF-8``

Zestaw znaków (kodowanie) który jest używany w całym frameworku.
Wartość tą możemy wyciągnąć z parametru ``kernel.charset``.

secret
~~~~~~

**type**: ``string`` **required**

Jest to ciąg znaków który powinien być unikalny dla Twojej aplikacji.
W praktyce używany jest do generowania tokenów CSRF, ale może być użyty
w każdym kontekście gdzie potrzebne jest użycie unikalnego ciągu znaków.
Wartość tą możemy wyciągnąć z parametru ``kernel.secret``.

ide
~~~

**type**: ``string`` **default**: ``null``

Jeśli IDE którego używasz to np. TextMate lub Mac Vim, Symfony może włączyć zamienianie
wszystkich ścieżek w wiadomościach wyjątków na linki, dzięki czemu Twoje IDE będzie mogło
je otworzyć jako plik.

Jeśli używasz TextMate lub Mac Vim, możesz w prosty sposób skorzystać z wbudowanych
wartości:

* ``textmate``
* ``macvim``

Możesz także określić niestandardowy link do pliku. Jeśli to zrobisz, wszystkie znaki
procentu (``%``) muszą zostać powtórzone w celu zinterpretowania tego znaku. Dla przykładu,
pełny ciąg znaków dla TextMate będzie wyglądać tak:

.. code-block:: yaml

    framework:
        ide:  "txmt://open?url=file://%%f&line=%%l"

Oczywiście, ponieważ każdy programista używa różnych IDE, lepszym rozwiązaniem jest ustawienie
tej wartości na poziomie systemu. Jest to możliwe poprzez ustawienie opcji ``xdebug.file_link_format``
w pliku PHP.ini. Jeśli opcja ta jest ustawiona, nie ma potrzeby definiowania opcji ``ide`` w
konfiguracji.

test
~~~~

**type**: ``Boolean``

Jeśli ten parametr konfiguracyjny jest ustawiony, wszystkie usługi związane z testowaniem
aplikacji zostaną załadowane. Ta opcja powinna być włączana na poziomie środowiska ``test``
(zwykle poprzez ``app/config/config_test.yml``). W celu uzyskania większej ilości informacji
zobacz :doc:`/book/testing`.

form
~~~~

csrf_protection
...............

session
~~~~~~~

lifetime
........

**type**: ``integer`` **default**: ``86400``

Wartość ta określa czas życia sesji - w sekundach.

templating

.. _ref-framework-assets-version:

assets_version
..............

**type**: ``string``

Ta opcja jest używana do *odświeżania* cache zasobów poprzez globalne dodanie
parametru do zapytania przy renderowaniu ścieżek do zasobów (np. ``/images/logo.png?v2`).
Ma to zastosowanie tylko do zasobów renderowanych przez funkcję Twig ``asset`` (lub
równoważną w PHP) oraz zasobów renderowanych przez assetic.

Załóżmy na przykład:

.. configuration-block::

    .. code-block:: html+jinja

        <img src="{{ asset('images/logo.png') }}" alt="Symfony!" />

    .. code-block:: php

        <img src="<?php echo $view['assets']->getUrl('images/logo.png') ?>" alt="Symfony!" />

Domyślnie, powyższy przykład wyrenderuje ścieżkę taką ścieżkę do Twojego obrazka ``/images/logo.png``.
Teraz, aktywujmy opcję ``assets_version``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            templating: { engines: ['twig'], assets_version: v2 }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:templating assets-version="v2">
            <framework:engine id="twig" />
        </framework:templating>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'templating'      => array(
                'engines' => array('twig'),
                'assets_version' => 'v2',
            ),
        ));

Teraz, ten sam zasób zostanie wyrenderowany jako ``/images/logo.png?v2``
Jeśli używasz tej funkcjonalności, **musisz** ręcznie inkrementować wartość
``assets_version`` przed każdym wdrożeniem aby parametr zapytania do ścieżki
się zmienił.

Możesz także kontrolować jak ma wyglądać parametr zapytania poprzez opcję
`assets_version_format`_.

assets_version_format
.....................

Opcja ta odnosi się do opcji `assets_version`_ i kontroluje jak ma być
skonstruowane zapytanie. Dla przykładu, jeśli ``assets_version_format``
jest ustawione na ``%s?version=%s`` oraz ``assets_version`` jest ustawione
na ``5``, wyrenderowany zasób będzie wyglądać tak ``/images/logo.png?version=5``.

Pełna Domyślna Konfiguracja
--------------------------

.. configuration-block::

    .. code-block:: yaml

        framework:

            # ogólna konfiguracja
            charset:              ~
            secret:               ~ # Required
            ide:                  ~
            test:                 ~

            # konfiguracja formularzy
            form:
                enabled:              true
            csrf_protection:
                enabled:              true
                field_name:           _token

            # konfiguracja esi
            esi:
                enabled:              true

            # konfiguracja profilera
            profiler:
                only_exceptions:      false
                only_master_requests:  false
                dsn:                  sqlite:%kernel.cache_dir%/profiler.db
                username:
                password:
                lifetime:             86400
                matcher:
                    ip:                   ~
                    path:                 ~
                    service:              ~

            # konfiguracja routera
            router:
                resource:             ~ # Required
                type:                 ~
                http_port:            80
                https_port:           443

            # konfiguracja sesji
            session:
                auto_start:           ~
                default_locale:       en
                storage_id:           session.storage.native
                name:                 ~
                lifetime:             86400
                path:                 ~
                domain:               ~
                secure:               ~
                httponly:             ~

            # konfiguracja szablonów
            templating:
                assets_version:       ~
                assets_version_format:  "%s?%s"
                assets_base_urls:
                    http:                 []
                    ssl:                  []
                cache:                ~
                engines:              # wymagane
                form:
                    resources:        [FrameworkBundle:Form]

                    # Przykład:
                    - twig
                loaders:              []
                packages:

                    # Prototype
                    name:
                        version:              ~
                        version_format:       ~
                        base_urls:
                            http:                 []
                            ssl:                  []

            # konfiguracja tłumaczeń
            translator:
                enabled:              true
                fallback:             en

            # konfiguracja walidacji
            validation:
                enabled:              true
                cache:                ~
                enable_annotations:   false

            # konfiguracja adnotacji
            annotations:
                cache:                file
                file_cache_dir:       %kernel.cache_dir%/annotations
                debug:                true
