:orphan:

Słownik
=======

.. glossary::
   :sorted:

   aktywa
      (*ang. assets*)
      W nomenklaturze Symfony pojecie aktywów jest pojeciem szerszym niż
      popularne pojecie zasobów i obejmuje nie tylko fizyczne pliki zasobów
      (CSS, JavaScript, obrazy itd.) ale też skrypty generowane na ich podstawie,
      czy tworzone "w locie".
      
   dystrybucja
      (*ang. distribution*)
      *Dystrybucja* jest zestawem instalacyjnych komponentów Symfony2,
      zawierajacym wybrane pakiett (*ang. bundles*), rozsądną strukturę katalogów,
      domyślną konfigurację i opcjonalny systemem konfiguracyjny.

   projekt
      (*ang. project*)
      *Projekt* jest katalogiem składającym się z *aplikacji*, zestawu pakietów,
      bibliotek dostawców, autoloadera i skryptów kontrolera wejścia.

   aplikcja
      (*ang. application*)
      Pojecie *aplikacji* jest używane w tej dokumentacji w dwóch kontekstach.
      Pierwszy, to tradycyjne znaczenie `aplikacji internetowej`_.
      Drugi, to katalog zawierający *konfigurację* dla danego zestawu
      *pakietów*.

   pakiet
      (*ang. bundle*)
      *Pakiet* jest katalogiem zawierającym zestaw plików (plików PHP,
      arkuszy atylów, plików JavaScript, obrazów ...) które *implementują*
      pojedyńczą funkcjonalność (blog, forum itp.). W Symfony2, (*prawie*)
      wszystko jest umieszczone w pakietach. (zobacz :ref:`page-creation-bundles`).
      W PHP koncepcja *pakietu*, od wersji PHP 5.3, odnosi się do *przestrzeni
      nazw* - każdy pakiet Symfony dwa tworzy własna *przestrzeń nazw*.  

   kontroler wejścia
   kontroler frontonowy
      (*ang. front controller*)
      *Kontroler wejścia* jest krótkim skryptem PHP umieszczonym w katalogu
      sieciowym projektu (np. web). Zazwyczaj *wszystkie* żądania są
      obsługiwane przez wykonanie jednego i tego samego kontrolera wejściowego,
      którego zadaniem jest zainicjowaie aplikacji Symfony.

   kontroler
      (*ang. controller*)
      *Kontroler* to funkcja PHP zawierająca całą logikę niezbędną do zwrócenia 
      obiektu ``Response``, który reprezentuje daną stronę.
      Zazwyczaj trasa jest odwzorowywana na kontroler, który następnie używa
      informacji z żądania HTTP do przetworzenia danych, przetwarza akcje
      i ostatecznie konstruuje i zwraca obiekt ``Response``. Należy zaznaczyć,
      że w polskiej literaturze poświęconej Symfony [1]_ termin *kontroler*
      odnoszony jest czasem do obiektu zawierającego metody, które tu określone
      zostały jako *kontroler* (np. ``DefaultController) a nie do tych metod.
      Tutaj *kontrolerem* będziemy nazywać, zgodnie z naszą defnicją, metodę
      (funkcję) wykonującą opisane tu działanie.    

   usługa
      (*ang. service*)
      *Usługa* to ogólny termin dla każdego obiektu PHP który wykonuje określone
      zadanie. Usługa zazwyczaj stosowana jest "globalnie", jako obiekt
      nawiązujący połączenie z bazą danych lub obiekt wysyłający wiadomości
      email. W Symfony2 usługi są często konfigurowane i pobierane z kontenera
      usług. Aplikacja posiadająca wiele oddzielnych usług nazywana jest
      aplikacją o `architekturze zorientowanej na usługi`_.

   kontener usługi
      (*ang. service container*)
      *Kontroler usług*, zwany też *kontenerem DI* (*od. ang. Dependency
      Injection Container*), jest specjalnym obiektem, który zarząda instancją
      usług wewnątrz aplikacji. Programista może *poinstruować* kontener usług
      (poprzez konfigurację) jak utworzyć usługi, zamiast tworzyć je bezpośrednio.
      Kontener usłu zajmuje się leniwym tworzeniem instancji i wstrzykiwaniem
      zależnych usług. Zobacz do rozdziału :doc:`/book/service_container`.
        
   specyfikacja HTTP
      (*ang. HTTP Specification*)
      *Specyfikacja Http* (Http Specification) jest dokumentem opisującym
      "Hypertext Transfer Protocol" - zbiór zasad leżących u podstaw klasycznej
      komunikacji żądanie-odpowiedź dla architektury klient-serwer.
      Specyfikacja definiuje format używany dla żądania (Request) oraz odpowiedzi
      (Response) jak i możliwe nagłówki HTTP które mogą one posiadać.
      Więcej informacji mozna znaleźć w artykule
      `HTTP`_ traktujący o `HTTP 1.1 RFC`_.

   środowisko
      (*ang. environment*)
      *Środowisko* to specyficzna konfiguracja aplikacji reprezentowana przez
      określone oznaczenie (np. ``prod`` lub ``dev``). Ta sama aplikacja
      może być uruchamiana na tej samej maszynie używając różnej konfiguracji
      poprzez uruchamianie aplikacji w różnych środowiskach. Jest to użyteczne
      ponieważ pozwala pojedyńczej aplikacji posiadać środowisko ``dev``
      dostosowane do debugowania oraz środowisko ``prod`` które jest zoptymalizowane
      pod kontem szybkości.
        
   dostawca
      (*ang. vendor*)
      *Dostawca* to ktoś dostarczający biblioteki PHP i pakiety dołączne do Symfony2.
      Pomimo skojarzenia tego słowa z kwestiami handlowymi (vendor w jezyku angielskim
      oznacza dosłownie "sprzedawcę"), dostawca w Symfony bardzo często (nawet
      zazwyczaj) dołącza bezpłatne oprogramowanie. Każda biblioteka którą chcesz
      dodać do projektu Symfony2 powinna znaleźć się w katalogu``vendor``.
      Zobacz :ref:`Architektura: Stosowanie "dostawców" <using-vendors>`

   Acme
      (*nazwa własna*)
      *Acme* jest prostą, przykładową nazwą firmy użytej w demo Symfony oraz dokumentacji.
      Jest użyta w przestrzeni nazw gdzie zwykle stosowana jest nazwa Twojej firmy
      (np. ``Acme\BlogBundle``).

   akcja
      (*ang. action*)
      *Akcja* jest funkcją lub metodą PHP która jest wykonywana, na przykład,
      gdy zostaje dopasowana przekazana trasa. Termin *akcja* jest synonimem z słowa
      *kontroler*, choć kontroler może również odnosić się do całej klasy PHP która
      zawiera kilka akcji. Zobacz :doc:`Rozdział o Kontrolerze </book/controller>`.

   zasób
      (*ang. asset*)
      *Zasób* jest komponentem aplikacji internetowej, bedącym plikiem takim jak
      CSS, JavaScript, obraz czy wideo. Mogą one być umiejscowione bezpośrednio
      w katalogu projektu ``web``, lub publikowane do katalogu ``web`` z :term:`pakietu`
      przez wykonanie polecenia ``assets:install`` w terminalu.

   Kernel
      (*nazwa własna*)
      *Kernel* jest rdzeniem Symfony2. Obiekt *Kernel* obsługuje zapytania
      HTTP używając wszystkich pakietów oraz bibliotek zarejestrowanych w nim.
      Zobacz :ref:`Architektura: Katalog app/ <the-app-dir>` oraz rozdział
      :doc:`/book/internals`

   zapora
      (*ang. firewall*)
      W Symfony2 *zapora* to nie to samo, co *zapora sieciowa*. Jest to mechanizm
      uwierzytelniania użytkowników (tzn. obsługuje proces identyfikacji użytkowników),
      albo dla całej aplikacji albo tylko jej części. Zobacz rozdział
      :doc:`/book/security`.

   Yaml
      (*nazwa własna*)
      *YAML* jest to uniwersalny język ustrukturyzowanego reprezentowania danych
      (tej samej klasy co XML), lekki i przejrzysty, szeroko stosowany w plikach
      konfiguracyjnych Symfony 2. Zobacz rozdział :doc:`/components/yaml/introduction` 
      oraz artykuł Wikipedii `YAML`_.


.. _`architekturze zorientowanej na usługi`: http://pl.wikipedia.org/wiki/Architektura_zorientowana_na_us%C5%82ugi
.. _`HTTP`: http://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`aplikacji internetowej`: http://pl.wikipedia.org/wiki/Aplikacja_(informatyka)
.. _`YAML`: http://pl.wikipedia.org/wiki/YAML
.. [1] W.Gajda "Symfony 2 od podstaw" Helion 2012