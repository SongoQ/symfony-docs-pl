Jak stworzyć konsolę/linię poleceń
=================================
Symfony2 posiada komponent Console, który pozwala Ci na stworzenie linii poleceń.
Twoja konsola może zostać użyta do każdych powtarzających się czynności,
takich jak zadania crona, importy, oraz inne zadania w tle.

Tworzenie podstawowych Poleceń
------------------------------

Aby linia poleceń była dostępna automatycznie w Symfony2, utwórz folder ``Command`` w środku swojego 
bundla oraz stwórz plik php z rozszerzeniem ``Command.php`` dla każdego polecenia do którego chcesz mieć dostęp.
Dla przykładu, jeśli chcesz rozszerzyć bundle ``AcmeDemoBundle`` (dostępny w edycji Symfony Standard Edition)
aby powitano naz z linii poleceń, utwórz plik ``GreetCommand.php`` oraz dodaj w jego treści::

    // src/Acme/DemoBundle/Command/GreetCommand.php
    namespace Acme\DemoBundle\Command;

    use Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends ContainerAwareCommand
    {
        protected function configure()
        {
            $this
                ->setName('demo:greet')
                ->setDescription('Greet someone')
                ->addArgument('name', InputArgument::OPTIONAL, 'Who do you want to greet?')
                ->addOption('yell', null, InputOption::VALUE_NONE, 'If set, the task will yell in uppercase letters')
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Hello '.$name;
            } else {
                $text = 'Hello';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
            }

            $output->writeln($text);
        }
    }

Dla przetestowania polecenia z konsoli wywołaj polecenie

.. code-block:: bash

    app/console demo:greet Fabien

Polecenie to wypiszę w linii poleceń:

.. code-block:: text

    Hello Fabien

Możesz także użyć opcji ``--yell`` aby powiększyć wszystkie litery:

.. code-block:: bash

    app/console demo:greet Fabien --yell

Wyświetli się::

    HELLO FABIEN

Kolorowanie oraz Wyjście
~~~~~~~~~~~~~~~~~~~~~~~~

Kiedykolwiek wypisujesz tekst, możesz go otoczyć specjalnymi tagami aby użyć kolorów::

    // zielony tekst
    $output->writeln('<info>foo</info>');

    // żółty tekst
    $output->writeln('<comment>foo</comment>');

    // czarny tekst oraz błękitne tło
    $output->writeln('<question>foo</question>');

    // biały tekst oraz czerwone tło
    $output->writeln('<error>foo</error>');

Korzystanie z Argumentów Konsolii
---------------------------------

Jednym z najbardziej interesujących elementów poleceń są argumenty oraz opcje,
które możesz udostępnić. Argumenty są ciągami znaków - oddzielone spacjami - które przychodzą 
zaraz po nazwie polecenia. Są one uporządkowane, oraz mogą być opcjonalne lub wymagane. Dla przykładu,
dodaj opcjonalny argument ``last_name`` oraz wymagany argument ``name`` do polecenia::

    $this
        // ...
        ->addArgument('name', InputArgument::REQUIRED, 'Who do you want to greet?')
        ->addArgument('last_name', InputArgument::OPTIONAL, 'Your last name?')
        // ...

Masz teraz dostęp do argumentu ``last_name`` w swoim poleceniu poprzez::

    if ($lastName = $input->getArgument('last_name')) {
        $text .= ' '.$lastName;
    }

Polecenie może być używane w jeden z następujących sposobów:

.. code-block:: bash

    app/console demo:greet Fabien
    app/console demo:greet Fabien Potencier

Używanie Opcji Polecenia
------------------------
W przeciwieństwie do argumentów, opcje nie są uporządkowane (możesz je przekazywać w dowolnej kolejności)
oraz są zdefiniowane z dwoma kreskami (np. ``--yell`` - możesz także zdefiniować jednoliterowy skrót który możesz
wywoływać z jedną kreską np. ``-y``). Opcje są *zawsze* opcjonalne, i mogą być ustawione na akceptowanie wartości (np. 
``dir=src``) lub jako flagi logiczne bez wartości (np. ``yell``).

.. tip::
    
    Możliwe jest także zdefiniowanie że opcja będzie *opcjonalnie* przyjmowała wartość 
    (więc opcja ``--yell`` lub ``yell=loud`` będzie poprawna). Opcje mogą być skonfigurowane
    tak aby przyjmować tablicę wartości.

Dla przykładu, zdefiniuj opcję w poleceniu do ustawienia ile razy linia z wiadomością powinna zostać wyświetlona::

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED, 'How many times should the message be printed?', 1)

Następnie, użyj tej opcji w poleceniu do wyświetlenia kilku wierszy:

.. code-block:: php

    for ($i = 0; $i < $input->getOption('iterations'); $i++) {
        $output->writeln($text);
    }

Teraz gdy będziesz uruchamiał polecenie, opcjonalnie możesz zdefiniować wartość ``--iterations``:

.. code-block:: bash

    app/console demo:greet Fabien

    app/console demo:greet Fabien --iterations=5

Pierwszy przykład wyświetli tylko jedną linię, ponieważ ``iterations`` jest puste i domyślną wartością jest ``1``
(ostatni argument metody ``addOption``). Drugi argument wyświetli wiadomośc pięć razy.

Przypomnijmy że nie jest ważna kolejność w jakiej przekażemy opcje.
Tak więc poniższe przykłady zadziałają tak samo:

.. code-block:: bash

    app/console demo:greet Fabien --iterations=5 --yell
    app/console demo:greet Fabien --yell --iterations=5

Pytanie Użytkownika o Informacje
--------------------------------

Podczas tworzenia poleceń, masz możliwość do zbierania informacji od użytkownika
poprzez pytanie jego/jej. Dla przykładu, przypuśćmy że chcesz potwierdzić akcję przed jej wykonaniem.
Dodaj następujące ustawienie do swojego polecenia::

    $dialog = $this->getHelperSet()->get('dialog');
    if (!$dialog->askConfirmation($output, '<question>Continue with this action?</question>', false)) {
        return;
    }

W tym przypadku, użytkownik zostanie zapytany "Continue with this action", i jeśli nie odpowie ``y``,
zadanie zostanie zatrzymane. Trzeci argument do metody ``askConfirmation`` jest domyślną zwracaną wartością
jeśli user nic nie wprowadzi.

Możesz także zadawać pytania które wymagają bardziej skomplikowanych odpowiedzi aniżeli tak/nie.
Dla przykładu, jeśli musisz znać nazwę czegoś, możesz zrobić to tak::

    $dialog = $this->getHelperSet()->get('dialog');
    $name = $dialog->ask($output, 'Please enter the name of the widget', 'foo');

Testowanie Poleceń
------------------

Symfony2 oferuje kilka narzędzi które pomogą Ci w testowaniu poleceń.
Najbardziej użyteczną jest klasa :class:`Symfony\\Component\\Console\\Tester\\CommandTester`.
Używa ona specjalnych klas wejścia i wyjścia dla łatwego testowania bez użycia prawdziwej konsoli::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Bundle\FrameworkBundle\Console\Application;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            // mock the Kernel or create one depending on your needs
            $application = new Application($kernel);

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getFullName()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

Metoda :method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay`
zwraca co było by zwrócone podczas normalnego wywołania polecenia w konsoli.

.. tip::

    Możesz także przetestować całą aplikację konsolową przy użyciu
    :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester`.

Pobieranie Usług z Service Container
------------------------------------

Poprzez używanie jako klasy bazowej klasy :class:`Symfony\Bundle\FrameworkBundle\Command\ContainerAwareCommand` 
dla polecenia (zamiast bardziej prostej klasy :class:`Symfony\Component\Console\Command\Command`), masz dostęp do 
kontenera usług (service container). Innymi słowy, masz dostęp do wszystkich skonfigurowanych usług.
Dla przykładu, możesz łatwo rozszerzyć polecenie aby korzystało z tłumaczeń::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $name = $input->getArgument('name');
        $translator = $this->getContainer()->get('translator');
        if ($name) {
            $output->writeln($translator->trans('Hello %name%!', array('%name%' => $name)));
        } else {
            $output->writeln($translator->trans('Hello!'));
        }
    }

Wywołanie istniejącego Polecenia
--------------------------------

Jeśli polecenie zależy od wywołania innego polecenia przed nim,
zamiast pytać użytkownika o kolejność wywołań,
możesz wywołać je samodzielnie.
Jest to przydatne jeśli chcesz stworzyć polecenie które tylko ma wywoływać inne polecenia
(dla przykładu, wszystkie polecenia które mają być uruchamiane gdy zmieni się kod na serwerze produkcyjnym:
czyszczenie cache, generowanie proxy Doctrine2, generowania dumpów Assetic, ...).

Wywoływanie jednego polecenia z drugiego jest bardzo proste::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $command = $this->getApplication()->find('demo:greet');

        $arguments = array(
            'command' => 'demo:greet',
            'name'    => 'Fabien',
            '--yell'  => true,
        );

        $input = new ArrayInput($arguments);
        $returnCode = $command->run($input, $output);

        // ...
    }

Po pierwsze, :method:`Symfony\\Component\\Console\\Command\\Command::find` polecenie które chcesz wywołać poprzez przekazanie jego nazwy.

Następnie, musisz utworzyć :class:`Symfony\\Component\\Console\\Input\\ArrayInput` z argumentami oraz opcjami które chcesz przekazać do polecenia.

W końcu, wywołujesz metodę ``run()`` która wywołuje polecenie oraz zwraca kod zwrócony przez polecenie (``0`` jeśli wszystko poszło dobrze,
oraz każdą inną liczbę w przeciwnym wypadku).

.. note::

    W większości przypadków wywoływanie polecenia z kodu który nie jest wywoływany w linii poleceń
    nie jest dobrym pomysłem z kilku powodów. Po pierwsze, wyjście polecenia jest zoptymalizowane dla linii poleceń.
    Ale co ważniejsze, możesz zacząć myśleć o poleceniu jak o kontrolerze; powinien używać modelu
    do wykonania zadania i zwróceniu wyniku do użytkownika. Więc, zamiast wywoływać polecenie z Webu,
    zrefaktoryzuj Swój kod i przenieś logikę do nowej klasy.
