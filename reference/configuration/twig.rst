.. index::
   pair: Twig; konfiguracja

TwigBundle - Konfiguracja
=========================

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        twig:
            exception_controller:  twig.controller.exception:showAction
            form:
                resources:

                    # Domyślnie:
                    - form_div_layout.html.twig

                    # Przykład:
                    - MyBundle::form.html.twig
            globals:

                # Przykłady:
                foo:                 "@bar"
                pi:                  3.14

                # Opcje przykładowe, ale najłatwiej stosować jest to co widać powyżej
                some_variable_name:
                    # id usługi, którym powinna być wartość
                    id:                   ~
                    # ustawienia dla usługi, lub pozostawić puste
                    type:                 ~
                    value:                ~
            autoescape:                ~

            # Ponizszy kod dodano w Symfony 2.3.
            # Zobacz http://twig.sensiolabs.org/doc/recipes.html#using-the-template-name-to-set-the-default-escaping-strategy
            autoescape_service:        ~ # Example: @my_service
            autoescape_service_method: ~ # use in combination with autoescape_service option
            base_template_class:       ~ # Example: Twig_Template
            cache:                     "%kernel.cache_dir%/twig"
            charset:                   "%kernel.charset%"
            debug:                     "%kernel.debug%"
            strict_variables:          ~
            auto_reload:               ~
            optimizations:             ~

    .. code-block:: xml
       :linenos:

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/doctrine/twig-1.0.xsd">

            <twig:config auto-reload="%kernel.debug%" autoescape="true" base-template-class="Twig_Template" cache="%kernel.cache_dir%/twig" charset="%kernel.charset%" debug="%kernel.debug%" strict-variables="false">
                <twig:form>
                    <twig:resource>MyBundle::form.html.twig</twig:resource>
                </twig:form>
                <twig:global key="foo" id="bar" type="service" />
                <twig:global key="pi">3.14</twig:global>
            </twig:config>
        </container>

    .. code-block:: php
       :linenos:

        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'MyBundle::form.html.twig',
                )
             ),
             'globals' => array(
                 'foo' => '@bar',
                 'pi'  => 3.14,
             ),
             'auto_reload'         => '%kernel.debug%',
             'autoescape'          => true,
             'base_template_class' => 'Twig_Template',
             'cache'               => '%kernel.cache_dir%/twig',
             'charset'             => '%kernel.charset%',
             'debug'               => '%kernel.debug%',
             'strict_variables'    => false,
        ));

Konfiguracja
------------

.. _config-twig-exception-controller:

exception_controller
....................

**typ**: ``string`` **domyślnie**: ``twig.controller.exception:showAction``

Kontroler ten jest aktywowany po zgłoszeniu w aplikacji wyjątku. Domyślny kontroler
(:class:`Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController`)
odpowiada za renderowanie szablonów dla różnych stanów aplikacji w warunkach błędu
(zobacza :doc:`/cookbook/controller/error_pages`). Modyfikowanie tej opcji jest zaawansowane.
Jeśli trzeba dostosować stronę błędu, to lepiej użyć działań omówionych na poprzedniej
stronie. Jeśli trzeba wykonać jakieś zachowanie na wyjątku, to należy dodać detektor
nasłuchujący (*ang. listner*) do zdarzenia ``kernel.exception`` (zobacz :ref:`dic-tags-kernel-event-listener`).
