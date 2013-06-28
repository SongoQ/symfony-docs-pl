.. index::
    single: pamięć podręczna; Varnish

Jak używać Varnish do przyspieszania stron www
==============================================

Ponieważ mechanizm buforowania (ang. cache) Symfony2 używa standardowych
nagłówków buforowania (ang. cache headers) protokołu HTTP, :ref:`symfony-gateway-cache`,
może on zostać z łatwością zastąpiony przez jakikolwiek inny serwer "reverse proxy".
Varnish to potężny, otwartoźródłowy akcelerator HTTP, który jest w stanie
zarówno obsłużyć buforowane treści szybko, jak i zapewnić wsparcie dla
języka znaczników :ref:`Edge Side Includes<edge-side-includes>`.

.. index::
    single: Varnish; konfiguracja

Konfiguracja
------------

Jak wspomniano wcześniej, framework Symfony2 jest wystarczająco inteligentny, aby wykryć
czy komunikacja przebiegała z udziałem serwera "reverse proxy" rozpoznającego ESI,
czy też nie. Wszystko zadziała z automatu, jeśli używano serwera "reverse proxy"
Symfony2, należy jednak dokonać dodatkowej konfiguracji w chwili, gdy planuje się pracę z
akceleratorem Varnish. Na szczęście Symfony2 opiera się na jeszcze innym standardzie
stworzonym przez Akamaï (`Architektura Edge`_), to też wskazówki konfiguracyjne w
tym rozdziale powinny być przydatne nawet wówczas, gdy nie planuje się korzystać z Symfony2.

.. note::

    Varnish wspiera tylko atrybuty ``src`` dla tagów ESI (atrybuty ``onerror``
    i ``alt`` są ignorowane).

Po pierwsze należy skonfigurować Varnish, aby powiadamiał o wsparciu dla ESI
poprzez dodanie nagłówka ``Surrogate-Capability`` do zapytań (ang. requests)
przekierowywanych do aplikacji zaplecza:

.. code-block:: text

    sub vcl_recv {
        // Dodaj nagłówek Surrogate-Capability, aby ogłosić wsparcie dla ESI.
        set req.http.Surrogate-Capability = "abc=ESI/1.0";
    }

Następnie należy zoptymalizować Varnish, aby analizował tylko te treści odpowiedzi
serwera (ang. Response), które zawierają przynajmniej jeden tag ESI poprzez
sprawdzanie nagłówka ``Surrogate-Control`` dodawanego automatycznie przez framework Symfony2:

.. code-block:: text

    sub vcl_fetch {
        /*
        Sprawdź potwierdzenie ESI
        i usuń nagłówek Surrogate-Control
        */
        if (beresp.http.Surrogate-Control ~ "ESI/1.0") {
            unset beresp.http.Surrogate-Control;

            // Dla Varnish >= 3.0
            set beresp.do_esi = true;
            // Dla Varnish < 3.0
            // esi;
        }
    }

.. caution::

    Kompresja z ESI nie była dostępna w Varnish aż do wersji 3.0 (proszę
    przeczytać `GZIP i Varnish`_). Jeśli nie używa się Varnish w wersji 3.0,
    należy umieścić dodatkowy serwer przed Varnish w celu wykonania poprawnej kompresji.

.. index::
    single: Varnish; unieważnienie

Unieważnienie pamięci podręcznej
--------------------------------

Nie powinno być konieczne unieważnienie danych pamięci podręcznej, gdyż proces
ten jest natywnie obsługiwany w modelach buforowania HTTP (proszę zobaczyć :ref:`http-cache-invalidation`).

Mimo to, Varnish może zostać skonfigurowany do obsługi specjalnej metody ``PURGE``
protokołu HTTP, która będzie w stanie unieważnić pamięć podręczną dla danego zasobu:

.. code-block:: text

    /*
     Połącz się z serwerem zaplecza
     na maszynie lokalnej na porcie 8080
     */
    backend default {
        .host = "127.0.0.1";
        .port = "8080";
    }

    sub vcl_recv {
        /*
        Domyślne zachowanie Varnish nie wspiera metody PURGE.
        Dopasuj zapytanie PURGE i natychmiast wykonaj sprawdzenie
        pamięci podręcznej, w innym przypadku Varnish przekieruje to zapytanie
        bezpośrednio do zaplecza, a tym samym ominie jakiekolwiek buforownie.
        */
        if (req.request == "PURGE") {
            return(lookup);
        }
    }

    sub vcl_hit {
        // Dopasuj zapytanie PURGE
        if (req.request == "PURGE") {
            // Wymuś ważność obiektu dla Varnish < 3.0
            set obj.ttl = 0s;
            // Dokonaj właściwego czyszczenia dla for Varnish >= 3.0
            // czyszczenie;
            error 200 "Purged";
        }
    }

    sub vcl_miss {
        /*
        Dopasuj zapytanie PURGE i
        oznacz, że nie było zapisane w pamięci podręcznej
        */
        if (req.request == "PURGE") {
            error 404 "Not purged";
        }
    }

.. caution::

    Należy chronić metodę ``PURGE`` protokołu HTTP w celu uniknięcia sytuacji,
    w której przypadkowi ludzie wyczyściliby dane z pamięci podręcznej. Można to
    zrobić poprzez ustawienie list dostępu:

    .. code-block:: text

        /*
         Połącz się z serwerem zaplecza
         na lokalnej maszynie na porcie 8080
         */
        backend default {
            .host = "127.0.0.1";
            .port = "8080";
        }

        // Lista dostępu może zawierać adresy IP, podsieci i nazwy hostów
        acl purge {
            "localhost";
            "192.168.55.0"/24;
        }

        sub vcl_recv {
            // Dopasuj zapytanie PURGE, aby zapobiec pominięciu procesu buforowania
            if (req.request == "PURGE") {
                // Dopasuj adres IP klienta do listy dostępu
                if (!client.ip ~ purge) {
                    // Odmowa dostępu
                    error 405 "Not allowed.";
                }
                // Przygotuj sprawdzenie pamięci podręcznej
                return(lookup);
            }
        }

        sub vcl_hit {
            // Dopasuj zapytanie PURGE
            if (req.request == "PURGE") {
                // Wymuś ważność obiektu dla Varnish < 3.0
                set obj.ttl = 0s;
                // Dokonaj właściwego czyszczenia dla for Varnish >= 3.0
                // czyszczenie;
                error 200 "Purged";
            }
        }

        sub vcl_miss {
            // Dopasuj zapytanie PURGE
            if (req.request == "PURGE") {
                // Oznacz, że obiekt nie jest zapisany w pamięci podręcznej
                error 404 "Not purged";
            }
        }

.. _`Architektura Edge`: http://www.w3.org/TR/edge-arch
.. _`GZIP i Varnish`: https://www.varnish-cache.org/docs/3.0/phk/gzip.html
