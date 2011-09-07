.. index::
    single: Cache; Varnish

Jak używać Varnish do przyśpieszenia Strony WWW
===============================================

Ponieważ Symfony2 używa standardowego cachowania nagłówkami HTTP,
:ref:`symfony-gateway-cache` może być łatwo zamieniony przez inny "reverse proxy".
Varnish jest potężnym, open-sourcowym, akceleratorem HTTP który jest wstanie obsłużyć
szybko zacechowaną treść, wspierany jest także :ref:`Edge Side Includes<edge-side-includes>`.

.. index::
    single: Varnish; configuration

Konfiguracja
------------

Jak widzieliśmy wcześniej, Symfony2 jest wystarczająco spryty aby wykryć czy komunikuje się 
z reverse proxy który rozpoznaje ESI lub też nie. Funkcjonalność ta dostępna jest od razu
jeśli korzystasz z reverse proxy dostępnego w Symfony2, ale aby działało to z Varnishem potrzebujesz
dodatkowej konfiguracji. Na szczęście, Symfony2 opiera się na jeszcze innym standardzie napisanym przez Akamaï (`Edge Architecture`_),
więc porady konfiguracyjne w tym dziale będą przydatne nawet jeśli nie korzystasz z Symfony2.

.. note::

    Varnish wspiera tylko atrybuty ``src`` dla tagów ESI (``onerror`` oraz ``alt``
    są ignorowane).

Po pierwsze, skonfiguruj Varnish tak aby wspierał ESI poprzez dodanie nagłówka ``Surrogate-Capability``
do zapytań (requests) przekierowanych do aplikacji:

.. code-block:: text

    sub vcl_recv {
        set req.http.Surrogate-Capability = "abc=ESI/1.0";
    }

Następnie, zoptymalizuj Varnish aby analizował tylko zawartość Response
jeśli zdefiniowany jest przynajmniej jeden tag ESI poprzez sprawdzenie nagłówka ``Surrogate-Control``
który to jest dodawany automatycznie przez Symfony2:

.. code-block:: text

    sub vcl_fetch {
        if (beresp.http.Surrogate-Control ~ "ESI/1.0") {
            unset beresp.http.Surrogate-Control;

            // for Varnish >= 3.0
            set beresp.do_esi = true;
            // for Varnish < 3.0
            // esi;
        }
    }

.. caution::

    Kompresja z ESI nie była wspierana w Varnish przed wersją 3.0 (przeczytaj `GZIP and Varnish`_).
    Jeśli nie używasz Varnish w wersji 3.0, umieść serwer przed Varnishem dla uzyskania kompresji.

.. index::
    single: Varnish; Invalidation

Unieważnienie Cache
-------------------

Nie powinieneś mieć potrzeby do unieważniania cache ponieważ funkcjonalność ta jest 
natywnie obsługiwana przez modele cache HTTP (zobacz :ref:`http-cache-invalidation`).

Mimo to, Varnish może być skonfigurowany do używania specjalnej metody HTTP ``PURGE``
która unieważni cache dla danego zasobu:

.. code-block:: text

    sub vcl_hit {
        if (req.request == "PURGE") {
            set obj.ttl = 0s;
            error 200 "Purged";
        }
    }

    sub vcl_miss {
        if (req.request == "PURGE") {
            error 404 "Not purged";
        }
    }

.. caution::

    Musisz chronić metodę HTTP ``PURGE`` przed nieautoryzowanym czyszczeniem cache.

.. _`Edge Architecture`: http://www.w3.org/TR/edge-arch
.. _`GZIP and Varnish`: https://www.varnish-cache.org/docs/3.0/phk/gzip.html
