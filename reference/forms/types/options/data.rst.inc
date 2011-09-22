data
~~~~

**typ**: mixed **domyślnie**: Domyślne, podstawowe pole obiektu (jeśli istnieje)

Podczas tworzenia formularza, każde pole wyświetla początkowo wartość
odpowiedniej właściwości formularza obiektu danej domeny (jeśli obiekt jest powiązany z
formularzem). Jeśli chcesz zmienić wartość początkową lub ustawić indywidualne
pole, możesz to zmienić w opcjach danych::

    $builder->add('token', 'hidden', array(
        'data' => 'abcdef',
    ));


