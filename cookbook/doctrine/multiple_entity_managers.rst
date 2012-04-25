Jak pracować z Kilkoma Menadżerami Encji
========================================

Możesz używać wielu menadżerów encji w aplikacji Symfony2. 
Jest to konieczne jeśli używasz kilku baz danych lub nawet z różnych 
zestawów encji. Innymi słowy, menadżer encji który łączy się z bazą 
będzie obsługiwał kilka encji podczas gdy inny menadżer encji który 
łączy się z inną bazą danych może obsługiwać resztę.

.. note::

    Używanie wielu menadżerów encji jest bardzo proste, ale bardziej zaawansowane i 
    w większości przypadków niepotrzebne. Upewnij się czy na pewno potrzebujesz 
    kilku menadżerów encji zanim wprowadzisz dodatkową złożoność w tej warstwie.

Poniższa konfiguracja pokazuje jak możesz skonfigurować kilka menadżerów encji:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            orm:
                default_entity_manager:   default
                entity_managers:
                    default:
                        connection:       default
                        mappings:
                            AcmeDemoBundle: ~
                            AcmeStoreBundle: ~
                    customer:
                        connection:       customer
                        mappings:
                            AcmeCustomerBundle: ~

W tym przypadku, zdefiniowałeś dwa menadżery encji i nazwałeś je ``default``
oraz ``customer``. Menadżer encji ``default`` zarządza encjami w ``AcmeDemoBundle`` oraz 
``AcmeStoreBundle``, a menadżer encji ``customer`` zarządza encjami w bundlu ``AcmeCustomerBundle``.

Podczas pracy z menadżerami encji, powinieneś jawnie wskazywać który menadżer encji Cię interesuje.
Jeśli pominiesz nazwę menadżera encji w odwoływaniu się do niego, zwracany jest
domyślny menadżer encji (np. ``default``)::

    class UserController extends Controller
    {
        public function indexAction()
        {
            // both return the "default" em
            $em = $this->get('doctrine')->getManager();
            $em = $this->get('doctrine')->getManager('default');
            
            $customerEm =  $this->get('doctrine')->getManager('customer');
        }
    }

Możesz teraz używać Doctrine tak jak dotychczas - używając menadżera encji ``default``
do utrzymania jak i pobierania encji którymi zarządza oraz menadżera encji ``customer``
do utrzymania i pobierania jego encji.
