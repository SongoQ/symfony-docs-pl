.. index::
   single: Debugging

Jak zoptymalizować środowisko programistyczne do debugowania
============================================================

Gdy pracujesz na projekcie opartym na Symfony, na swojej lokalnej maszynie,
powinieneś używać środowiska ``dev`` (``app_dev.php`` front kontroler). Konfiguracja tego środowiska
ma dwa główne cele:

 * Dania deweloperowi jak najdokładniejszych informacji jeśli coś pójdzie nie tak (web debug toolbar,
   strony wyjątków, profiler, ...);

 * Bycie jak najbardziej podobnym do środowiska produkcyjnego, w celu uniknięcia problemów podczas wdrażania projektu.

.. _cookbook-debugging-disable-bootstrap:

Wyłączenie pliku Bootstrap oraz Cache Klas
------------------------------------------

Aby uczynić środowisko produkcyjne tak szybkim jakim się da, Symfony tworzy 
duże pliki PHP w cache zawierające informacje o klasach PHP które Twój projekt potrzebuje przy każdym
wywołaniu strony. Jednak to zachowanie może mylić Twoje IDE lub też Twój debugger.
Ten przepis pokaże Ci jak zmienić zachowanie mechanizmu cache aby był bardziej przyjazny podczas debugowania kodu 
który obejmuje klasy Symfony.

Kontroler ``app_dev.php`` wygląda domyślnie tak::

    // ...

    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

Dla uszczęśliwienia swojego debugera, wyłącz cache dla klas PHP poprzez usunięcie odwołania do ``loadClassCache()``
oraz zamień wymagane elementy, jak poniżej::

    // ...

    // require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../vendor/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';
    require_once __DIR__.'/../app/autoload.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('dev', true);
    // $kernel->loadClassCache();
    $kernel->handle(Request::createFromGlobals())->send();

.. tip::

    Jeśli wyłączysz cache PHP, nie zapomnij go włączyć po zakończeniu debugowania.

Niektóre IDE nie lubią faktu że niektóre klasy trzymane są w różnych lokalizacjach.
Aby uniknąć tego problemu możesz ustawić w swoim IDE aby ignorował pliki cache PHP,
lub możesz zmienić rozszerzenie stosowane przez Symfony dla tych plików::

    $kernel->loadClassCache('classes', '.php.cache');
