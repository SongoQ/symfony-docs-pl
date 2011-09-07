.. index::
   single: Controller; As Services

Jak zdefiniować Kontrolery (Controllers) jako Usługi (Services)
===============================================================

W książce, dowiedziałeś się jak łatwo kontroler (controller) może być użyty jeśli rozszerza
klasę bazową :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller`.
Podczas gdy to działa dobrze, kontrolery mogą być także zdefiniowane jako usługi (services).

Aby odwołać się do kontrolera który zdefiniowany jest jako usługa, użyj notacji z pojedyńczym dwukropkiem (:).
Dla przykładu, przypuśćmy że zdefiniowaliśmy usługę zwaną ``my_controller`` i w usłudze
chcemy przekierować do metody zwanej ``indexAction()``::

    $this->forward('my_controller:indexAction', array('foo' => $bar));

Musisz użyć tej samej notacji podczas definiowania routingu ``_controller``::

.. code-block:: yaml

    my_controller:
        pattern:   /
        defaults:  { _controller: my_controller:indexAction }

Aby używać kontrolera w ten sposób, musi on być zdefiniowany w konfiguracji kontenera usług (service container).
Dla uzyskania większej ilości informacji, zobacz rozdział :doc:`Service Container</book/service_container>`.

Kiedy używasz kontrolera jako usługi, najprawdopodobniej nie będziesz rozszerzał bazowej klasy ``Controller``.
Zamiast korzystać z jego skróconych metod, będziesz bezpośrednio wywoływał usługi które potrzebujesz.
Na szczęście, jest to zwykle bardzo proste i bazowa klasa ``Controller`` sama w sobie jest doskonałym
źródłem w jaki sposób wykonywać wiele typowych zadań.

.. note::

    Zdefiniowanie kontrolera jako usługi zajmuje trochę więcej czasu. 
    Główną zaletą tego rozwiązania jest to że cały kontroler lub też jakakolwiek usługa 
    przekazana do kontrolera może być modyfikowania poprzez konfigurację kontenera usług.
    Jest to szczególnie przydatne przy opracowywaniu open-sourcowego bundla lub też bundla
    który będzie użyty w wielu różnych projektach. Więc, nawet jeśli nie zdefiniujesz swoich
    kontrolerów jako usługi, to często będziesz mógł zobaczyć tego typu rozwiązania
    w open-sourcowych bundlach Symfony2.
