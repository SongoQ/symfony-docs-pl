.. index::
   single: Event Dispatcher

Jak dostosować Zachowanie Metody bez używania Dziedziczenia
===========================================================

Robienie czegoś przed lub po Wywołaniu Metody
---------------------------------------------

Jeśli chcesz zrobić coś bezpośrednio, tuż przed, lub tuż po wywołaniu metody,
możesz wywołać (dispatch) zdarzenie (event) odpowiednio na początku lub na końcu
metody::

    class Foo
    {
        // ...

        public function send($foo, $bar)
        {
            // do something before the method
            $event = new FilterBeforeSendEvent($foo, $bar);
            $this->dispatcher->dispatch('foo.pre_send', $event);

            // get $foo and $bar from the event, they may have been modified
            $foo = $event->getFoo();
            $bar = $event->getBar();
            // the real method implementation is here
            // $ret = ...;

            // do something after the method
            $event = new FilterSendReturnValue($ret);
            $this->dispatcher->dispatch('foo.post_send', $event);

            return $event->getReturnValue();
        }
    }

W tym przykładzie wywoływane są dwa zdarzenia: ``foo.pre_send``, przed wykonaniem metody,
oraz ``foo.post_send`` po wywołaniu metody. Każde używa niestandardowej klasy zdarzenia (event)
do przekazania informacji do słuchacza (listener) tych dwóch zdarzeń. Klasy tych zdarzeń powinny być 
stworzone przez Ciebie oraz powinny, w tym przykładzie, mieć możliwość odczytania oraz
ustawienia zmiennych ``$foo``, ``$bar`` oraz ``$ret`` przez słuchaczy (listeners).

Dla przykładu, zakładając że ``FilterSendReturnValue`` posiada metodę ``setReturnValue``,
jeden słuchacz może wyglądać następująco:

.. code-block:: php

    public function onFooPostSend(FilterSendReturnValue $event)
    {
        $ret = $event->getReturnValue();
        // modify the original ``$ret`` value

        $event->setReturnValue($ret);
    }
