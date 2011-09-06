.. index::
   single: Event Dispatcher

Jak rozszerzyć Klasę bez użycia dziedziczenia
=============================================

Aby umożliwić kilku klasom do dodania metod do innej klasy,
możesz zdefiniować metodę magiczną ``__call()`` do klasy którą chcesz rozszerzyć,
tak jak poniżej:

.. code-block:: php

    class Foo
    {
        // ...

        public function __call($method, $arguments)
        {
            // create an event named 'foo.method_is_not_found'
            $event = new HandleUndefinedMethodEvent($this, $method, $arguments);
            $this->dispatcher->dispatch($this, 'foo.method_is_not_found', $event);

            // no listener was able to process the event? The method does not exist
            if (!$event->isProcessed()) {
                throw new \Exception(sprintf('Call to undefined method %s::%s.', get_class($this), $method));
            }

            // return the listener returned value
            return $event->getReturnValue();
        }
    }

Metoda ta używa specjalnej klasy ``HandleUndefinedMethodEvent`` która powinna także zostać stworzona.
Jest to klasa bazowa która może być użyta za każdym razem gdy chcesz użyć tego wzorca
rozszerzania klas:

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class HandleUndefinedMethodEvent extends Event
    {
        protected $subject;
        protected $method;
        protected $arguments;
        protected $returnValue;
        protected $isProcessed = false;

        public function __construct($subject, $method, $arguments)
        {
            $this->subject = $subject;
            $this->method = $method;
            $this->arguments = $arguments;
        }

        public function getSubject()
        {
            return $this->subject;
        }

        public function getMethod()
        {
            return $this->method;
        }

        public function getArguments()
        {
            return $this->arguments;
        }

        /**
         * Sets the value to return and stops other listeners from being notified
         */
        public function setReturnValue($val)
        {
            $this->returnValue = $val;
            $this->isProcessed = true;
            $this->stopPropagation();
        }

        public function getReturnValue($val)
        {
            return $this->returnValue;
        }

        public function isProcessed()
        {
            return $this->isProcessed;
        }
    }

Następnie, utwórz klasę która będzie nasłuchiwać (listen) zdarzenia (event) ``foo.method_is_not_found``
i *dodaj* metodę ``bar()``:

.. code-block:: php

    class Bar
    {
        public function onFooMethodIsNotFound(HandleUndefinedMethodEvent $event)
        {
            // we only want to respond to the calls to the 'bar' method
            if ('bar' != $event->getMethod()) {
                // allow another listener to take care of this unknown method
                return;
            }

            // the subject object (the foo instance)
            $foo = $event->getSubject();

            // the bar method arguments
            $arguments = $event->getArguments();

            // do something
            // ...

            // set the return value
            $event->setReturnValue($someValue);
        }
    }

Na koniec, dodaj nową metodę ``bar`` do klasy ``Foo`` poprzez rejestrację
instancji ``Bar`` z zdarzeniem ``foo.method_is_not_found``:

.. code-block:: php

    $bar = new Bar();
    $dispatcher->addListener('foo.method_is_not_found', $bar);
