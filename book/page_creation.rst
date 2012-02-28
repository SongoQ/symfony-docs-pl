.. index::
   single: Page creation

Tworzenie stron w Symfony2
==========================

Tworzenie nowej strony w Symfony2 to prosty, dwuetapowy proces:

* *Utwórz route*: Route definiuje URL (np. /o-nas) do Twojej strony oraz okreœla kontroler (który jest funkcj¹ PHP) który Symfony2 powinno wykonaæ, kiedy URL przychodz¹cego ¿¹dania pasuje do wzoru route'a.

* *Utwórz kontroler*: Kontroler jest funkcj¹ PHP, która pobiera przychodz¹ce ¿¹danie i transformuje je w obiekt *Odpowiedzi* Symfony2, który jest zwracany u¿ytkownikowi.


To proste podejœcie jest piêkne, gdy¿ pasuje do sposobu funkcjonowania Sieci. Ka¿da interakcja w Sieci jest inicjowana przez ¿¹danie HTTP. Zadaniem Twojej aplikacji jest po prostu zinterpretowaæ ¿¹danie i zwróciæ w³aœciw¹ odpowiedŸ HTTP.

Symfony2 stosuje t¹ filozofiê i dostarcza Ci narzêdzia i konwencje, które pomog¹ Ci utrzymaæ swoj¹ aplikacjê zorganizowan¹ wraz ze wzrostem liczby u¿ytkowników i z³o¿onoœci.

Brzmi wystarczaj¹co prosto? Zag³êbmy siê w tym!

Strona "Witaj Symfony!"
-----------------------

Zacznijmy od klasycznej aplikacji "Witaj Œwiecie!". Kiedy skoñczysz, u¿ytkownik bêdzie mia³ mo¿liwoœæ ujrzeæ osobiste pozdrowienie (np. "Witaj Symfony") wchodz¹c w poni¿szy URL:

.. code-block:: text

    http://localhost/app_dev.php/hello/Symfony

W rzeczywistoœci bêdziesz móg³ zast¹piæ *Symfony* innym imieniem, które ma byæ pozdrowione. Aby utworzyæ stronê, wykonaj prosty, dwuetapowy proces.