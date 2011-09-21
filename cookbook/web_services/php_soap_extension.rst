.. index::
    single: Web Services; SOAP

Jak Utworzyć Web Service SOAP w Kontrolerze Symfony2
====================================================

Ustawienie kontrolera aby zachowywał się jak serwer SOAP jest bardzo proste
dzięki kilku narzędziom. Musisz, oczywiście, mieć zainstalowane rozszerzenie
`PHP SOAP`_. Jako że rozszerzenie PHP SOAP nie może aktualnie generować WSDL,
musisz utworzyć go od zera lub też skorzystać z jakiegoś zewnętrznego generatora.

.. note::

    Jest kilka implementacji serwera SOAP dostępnych w PHP. Są to np.
    `Zend SOAP`_ oraz `NuSOAP`_. W naszych przykładach wykorzystujemy
    rozszerzenie PHP SOAP, ale ogólna idea powinna mieć zastosowanie
    w innych implementacjach.

SOAP działa przez wystawienie metod obiektu PHP zewnętrznemu podmiotowi
(np. osobie używającej usługi SOAP). Aby rozpocząć, utwórz klasę - ``HelloService`` -
która będzie reprezentować funkcjonalność wystawioną w usłudze SOAP.
W tym przypadku, usługa SOAP da możliwość klientowi na wywołanie metody ``hello``,
która wyśle e-mail::

    namespace Acme\SoapBundle;

    class HelloService
    {
        private $mailer;

        public function __construct(\Swift_Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        public function hello($name)
        {
            
            $message = \Swift_Message::newInstance()
                                    ->setTo('me@example.com')
                                    ->setSubject('Hello Service')
                                    ->setBody($name . ' says hi!');

            $this->mailer->send($message);


            return 'Hello, ' . $name;
        }

    }

Następnie, możesz poinstruować Symfony aby mógł utworzyć instancję tej klasy.
Klasa wysyłająca e-maile, została zaprojektowana tak aby akceptować instancję
``Swift_Mailer``. Używając Kontenera Usług (Service Container), możemy skonfigurować
Symfony aby tworzył ``HelloService`` poprawnie:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml    
        services:
            hello_service:
                class: Acme\DemoBundle\Services\HelloService
                arguments: [mailer]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
         <service id="hello_service" class="Acme\DemoBundle\Services\HelloService">
          <argument>mailer</argument>
         </service>
        </services>

Poniżej znajduje się przykład kontrolera zdolnego do obsługi zapytań SOAP.
Jeśli ``indexAction()`` jest dostępna poprzez routing ``/soap``, wtedy
dokument WSDL może zostać pobrany przez ``/soap?wsdl``.

.. code-block:: php

    namespace Acme\SoapBundle\Controller;
    
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloServiceController extends Controller 
    {
        public function indexAction()
        {
            $server = new \SoapServer('/path/to/hello.wsdl');
            $server->setObject($this->get('hello_service'));
            
            $response = new Response();
            $response->headers->set('Content-Type', 'text/xml; charset=ISO-8859-1');
            
            ob_start();
            $server->handle();
            $response->setContent(ob_get_clean());
            
            return $response;
        }
    }

Należy zwrócić uwagę na wywołanie ``ob_start()`` oraz ``ob_get_clean()``.
Metody te kontrolują `output buffering`_ który umożliwia Ci "złapać"
wywołania echo w ``$server->handle()``. Jest to wymagane ponieważ Symfony
oczekuje od Twojego kontrolera aby zwracał obiekt ``Response`` z ustawioną
"treścią" do zwrócenia. Musisz także pamiętać o ustawieniu nagłówka
"Content-Type" z wartością "text/xml", ponieważ takiego typu danych oczekuje
klient. A więc, wykorzystujesz ``ob_start()`` aby rozpocząć buforowanie STDOUT
oraz ``ob_get_clean()`` aby zrzucić wszystkie wywołania echo do treści obiektu
Response oraz wyczyszczenia bufora wyjścia. W końcu, jesteś gotowy aby zwrócić
obiekt ``Response``.

Poniżej jest przykład użycia usługi używającej klienta `NuSOAP`_.
Ten przykład zakłada że ``indexAction`` w kontrolerze powyżej jest dostępna
przez routing ``/soap``::

    $client = new \soapclient('http://example.com/app.php/soap?wsdl', true);
    
    $result = $client->call('hello', array('name' => 'Scott'));

Przykład WSDL poniżej.

.. code-block:: xml

    <?xml version="1.0" encoding="ISO-8859-1"?>
     <definitions xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" 
         xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
         xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/" 
         xmlns:tns="urn:arnleadservicewsdl" 
         xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" 
         xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/" 
         xmlns="http://schemas.xmlsoap.org/wsdl/" 
         targetNamespace="urn:helloservicewsdl">
      <types>
       <xsd:schema targetNamespace="urn:hellowsdl">
        <xsd:import namespace="http://schemas.xmlsoap.org/soap/encoding/" />
        <xsd:import namespace="http://schemas.xmlsoap.org/wsdl/" />
       </xsd:schema>
      </types>
      <message name="helloRequest">
       <part name="name" type="xsd:string" />
      </message>
      <message name="helloResponse">
       <part name="return" type="xsd:string" />
      </message>
      <portType name="hellowsdlPortType">
       <operation name="hello">
        <documentation>Hello World</documentation>
        <input message="tns:helloRequest"/>
        <output message="tns:helloResponse"/>
       </operation>
      </portType>
      <binding name="hellowsdlBinding" type="tns:hellowsdlPortType">
      <soap:binding style="rpc" transport="http://schemas.xmlsoap.org/soap/http"/>
      <operation name="hello">
       <soap:operation soapAction="urn:arnleadservicewsdl#hello" style="rpc"/>
       <input>
        <soap:body use="encoded" namespace="urn:hellowsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </input>
       <output>
        <soap:body use="encoded" namespace="urn:hellowsdl" 
            encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
       </output>
      </operation>
     </binding>
     <service name="hellowsdl">
      <port name="hellowsdlPort" binding="tns:hellowsdlBinding">
       <soap:address location="http://example.com/app.php/soap" />
      </port>
     </service>
    </definitions>


.. _`PHP SOAP`:          http://php.net/manual/en/book.soap.php
.. _`NuSOAP`:            http://sourceforge.net/projects/nusoap
.. _`output buffering`:  http://php.net/manual/en/book.outcontrol.php
.. _`Zend SOAP`:         http://framework.zend.com/manual/en/zend.soap.server.html
