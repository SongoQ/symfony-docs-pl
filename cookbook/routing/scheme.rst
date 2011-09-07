.. index::
   single: Routing; Scheme requirement

Jak zawsze wymusić w routingu używanie protokołu HTTPS
======================================================

Czasami chcesz zabezpieczyć niektóre routingi aby być pewnym że 
zawsze będą dostępne tylko przez protokół HTTPS. Komponent Routing 
umożliwia Ci wymuszenie typu HTTP poprzez wymaganą zmienną ``_scheme``:

.. configuration-block::

    .. code-block:: yaml

        secure:
            pattern:  /secure
            defaults: { _controller: AcmeDemoBundle:Main:secure }
            requirements:
                _scheme:  https

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="secure" pattern="/secure">
                <default key="_controller">AcmeDemoBundle:Main:secure</default>
                <requirement key="_scheme">https</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('secure', new Route('/secure', array(
            '_controller' => 'AcmeDemoBundle:Main:secure',
        ), array(
            '_scheme' => 'https',
        )));

        return $collection;

Powyższa konfiguracja wymusza na routingu ``secure`` aby zawsze używał HTTPS.

Gdy będziesz generował URL do ``secure``, i nawet jeśli aktualnm protokołem będzie HTTP,
Symfony automatycznie wygeneruje absolutny URL z protokołem HTTPS:

.. code-block:: text

    # If the current scheme is HTTPS
    {{ path('secure') }}
    # generates /secure

    # If the current scheme is HTTP
    {{ path('secure') }}
    # generates https://example.com/secure

Wymóg ten jest także egzekwowany w stosunku do przychodzących żądań (requests).
Jeśli spróbujesz dostać się do ``/secure`` poprzez HTTP, zostaniesz automatycznie 
przekierowany pod ten sam adres ale używając protokołu HTTPS.

Powyższy przykład używa ``https`` jako wartość ``_scheme``, ale możesz także 
wymusić aby URL korzystał zawsze z ``http``.

.. note::

    Komponent Security zapewna także inny sposób wymuszenia stosowania protokołu HTTP
    poprzez ustawienie ``requires_channel``. Alternatywa ta jest lepsza w celu
    zabezpieczenia "części" Twojej strony (wszystkie URLe w ``/admin``) lub też
    kiedy chcesz zabezpieczyć URLe zdefiniowane w obcych bundlach.
