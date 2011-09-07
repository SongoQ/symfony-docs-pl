.. index::
   single: Routing; Allow / in route parameter

Jak zezwolić na używanie znaku "/" w parametrach routingu
========================================================

Czasami możesz chcieć aby parametry URLi zawierały znak ``/``. 
Weźmy dla przykładu ``/hello/Fabien`` będzie pasował do tego routingu
ale już ``/hello/Fabien/Kris`` nie. Dzieje się tak ponieważ Symfony
używa tego znaku jako separatora części routingu.

Niniejszy poradnik pokaże Ci jak możesz zmodyfikować routing
aby ``/hello/Fabien/Kris`` pasowało do routingu ``/helo/{name}``,
gdzie ``{name}`` będzie zawierać ``Fabien/Kris``.

Konfiguracja Routingu
---------------------

Domyślnie komponenty routingu symfony, wymagają aby parametry 
były zgodne z wzorcem regexp ``[^/]+``. Oznacza to że mogą wystąpić w nim
wszystkie znaki oprócz ``/``.

Musisz wyraźnie zezwolić na używanie ``/`` jako części parametru
poprzez bardziej liberalny wzorzec regexp.

.. configuration-block::

    .. code-block:: yaml

        _hello:
            pattern: /hello/{name}
            defaults: { _controller: AcmeDemoBundle:Demo:hello }
            requirements:
                name: ".+"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_hello" pattern="/hello/{name}">
                <default key="_controller">AcmeDemoBundle:Demo:hello</default>
                <requirement key="name">.+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeDemoBundle:Demo:hello',
        ), array(
            'name' => '.+',
        )));

        return $collection;

    .. code-block:: php-annotations

        use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

        class DemoController
        {
            /**
             * @Route("/hello/{name}", name="_hello", requirements={"name" = ".+"})
             */
            public function helloAction($name)
            {
                // ...
            }
        }

To wszystko! Teraz, parametr ``{name}`` może zawierać znak ``/``.
