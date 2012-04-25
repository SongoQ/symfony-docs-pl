.. _doctrine-event-config:

Rejestracja "Event Listeners" oraz "Subscribers"
================================================

Doctrine posiada bogady system zdarzeń (event system) który odpala zdarzenia 
praktycznie przy każdej możliwej sytuacji w systemie. Dla Ciebie oznacza to 
że możesz tworzyć dowolne :doc:`services</book/service_container>` 
i powiedzieć Doctrine że ma zawiadamiać te obiekty o pewnych akcjach (np. ``preSave``)
wykonywanych w Doctrine. To może być użyteczne, dla przykładu, do stworzenia 
niezależnego indeksu do wyszukiwania, za każdym razem gdy zapisywany jest obiekt 
w Twojej bazie.

Doctrine definiuje dwa typy obiektów które mogą nasłuchiwać zdarzeń (events) Doctrine:
"listeners" i "subscribers". Oba są bardzo podobne, ale "listeners" są nieco prostsze.
Dla uzyskania większej ilości informacji, zobacz artykuł `The Event System`_ na stronie Doctrine.

Konfiguracja Listener/Subscriber
--------------------------------

Aby skonfigurować usługę aby działała jako "event listener" lub "subscriber" musisz tylko
:ref:`tag<book-service-container-tags>` z odpowiednią nazwą.
W zależności od sposobu użycia, możesz podłączyć "listener" do każdego połączenia DBAL oraz 
każdego menadżera ORM lub tylko do specyficznego połączenia DBAL oraz każdego menadżera który
używa tego połączenia.

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            dbal:
                default_connection: default
                connections:
                    default:
                        driver: pdo_sqlite
                        memory: true

        services:
            my.listener:
                class: Acme\SearchBundle\Listener\SearchIndexer
                tags:
                    - { name: doctrine.event_listener, event: postPersist }
            my.listener2:
                class: Acme\SearchBundle\Listener\SearchIndexer2
                tags:
                    - { name: doctrine.event_listener, event: postPersist, connection: default }
            my.subscriber:
                class: Acme\SearchBundle\Listener\SearchIndexerSubscriber
                tags:
                    - { name: doctrine.event_subscriber, connection: default }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine">

            <doctrine:config>
                <doctrine:dbal default-connection="default">
                    <doctrine:connection driver="pdo_sqlite" memory="true" />
                </doctrine:dbal>
            </doctrine:config>

            <services>
                <service id="my.listener" class="Acme\SearchBundle\Listener\SearchIndexer">
                    <tag name="doctrine.event_listener" event="postPersist" />
                </service>
                <service id="my.listener2" class="Acme\SearchBundle\Listener\SearchIndexer2">
                    <tag name="doctrine.event_listener" event="postPersist" connection="default" />
                </service>
                <service id="my.subscriber" class="Acme\SearchBundle\Listener\SearchIndexerSubscriber">
                    <tag name="doctrine.event_subscriber" connection="default" />
                </service>
            </services>
        </container>

Tworzenie Klasy "Listener"
--------------------------

W poprzednim przykładzie, usługa ``my.listener`` była skonfigurowana jako "listener" Doctrine
na zdarzeniu (event) ``postPersist``. Ta klasa za tą usługą musi posiadać metodę ``postPersist``, 
która będzie wywoływana kiedy zdarzenie jest rzucane::

    // src/Acme/SearchBundle/Listener/SearchIndexer.php
    namespace Acme\SearchBundle\Listener;
    
    use Doctrine\ORM\Event\LifecycleEventArgs;
    use Acme\StoreBundle\Entity\Product;
    
    class SearchIndexer
    {
        public function postPersist(LifecycleEventArgs $args)
        {
            $entity = $args->getEntity();
            $entityManager = $args->getManager();
            
            // perhaps you only want to act on some "Product" entity
            if ($entity instanceof Product) {
                // do something with the Product
            }
        }
    }

W każdym zdarzeniu, masz dostęp do obiektu ``LifecycleEventArgs``, który
daje Ci dostęp do samego obiektu encji zdarzenia oraz do menadżera encji.

Jest jedna ważna rzecz na którą musisz zwrócić uwagę, "listener" będzie nasłuchiwał
dla *wszystkich* encji w Twojej aplikacji. Więc, jeśli jesteś zainteresowany obsługą
tylko wybranych typów encji (np. ``Product`` a nie ``BlogPost``), powinienieś 
sprawdzać nazwę klasy encji w swojej metodzie (tak jak jest to pokazane wyżej).

.. _`The Event System`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/events.html
