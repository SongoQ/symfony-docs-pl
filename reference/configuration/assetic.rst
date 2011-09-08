.. index::
   pair: Assetic; Configuration Reference

AsseticBundle - Konfiguracja
============================

Pełna Domyślna Konfiguracja
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        assetic:
            debug:                true
            use_controller:       true
            read_from:            %kernel.root_dir%/../web
            write_to:             %assetic.read_from%
            java:                 /usr/bin/java
            node:                 /usr/bin/node
            sass:                 /usr/bin/sass
            bundles:

                # Domyślnie (wszystkie aktualnie zarejestrowane bundle):
                - FrameworkBundle
                - SecurityBundle
                - TwigBundle
                - MonologBundle
                - SwiftmailerBundle
                - DoctrineBundle
                - AsseticBundle
                - ...

            assets:

                # Prototype
                name:
                    inputs:               []
                    filters:              []
                    options:

                        # Prototype
                        name:                 []
            filters:

                # Prototype
                name:                 []
            twig:
                functions:

                    # Prototype
                    name:                 []
