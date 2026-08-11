# Appunti C: Multithreading, Struct e Puntatori

## PARTE 1: Teoria del Multithreading (Da ripristino)

### 1. Parallelismo (Parallelism)
È quando più cose vengono eseguite **letteralmente nello stesso identico istante**. Affinché ci sia vero parallelismo, hai bisogno di un computer con più Core fisici (es. un processore Quad-Core può eseguire 4 thread fisicamente in parallelo).

### 2. Concorrenza (Concurrency / Simultanealism)
È l'illusione del parallelismo. Succede quando ci sono più thread attivi che si "alternano" velocemente. Anche se hai un solo Core fisico, il Sistema Operativo dà un millisecondo al Thread A, poi un millisecondo al Thread B. L'utente percepisce che i due programmi stanno andando "simultaneamente", ma sotto il cofano si stanno dando il cambio.

### 3. Context Switch (Cambio di Contesto)
È l'operazione esatta con cui il Sistema Operativo (lo Scheduler) mette in pausa un thread per dare spazio a un altro.
* Il SO salva tutto lo stato del Thread A (registri, variabili).
* Carica lo stato del Thread B e lo fa ripartire.
**Il Pericolo:** Il Context Switch è *imprevedibile*. Può avvenire a metà di un'operazione critica (es. proprio mentre stai allungando la mano per prendere un dongle). 

### 4. Mutex (Mutual Exclusion)
Visto che il Context Switch è imprevedibile, il Mutex è la nostra difesa. È un "lucchetto".
Quando un thread fa `pthread_mutex_lock`, entra in una stanza e chiude la porta. 
Se avviene un Context Switch e il thread si addormenta nella stanza, **nessun altro thread potrà entrare**. Gli altri si bloccheranno fuori dalla porta aspettando che il primo si svegli e faccia `pthread_mutex_unlock`. Previene incidenti disastrosi quando più thread toccano le stesse variabili.

---

## PARTE 2: Struct, Puntatori e Regola d'Oro (`.` vs `->`)

### 1. Blueprint vs Memoria (Dichiarazione vs Istanziazione)
Quando scrivi una struct in un file `.h`:
```c
typedef struct s_rules {
    int time_to_compile;
} t_rules;
```
**NON stai occupando memoria.** Stai solo creando un "progetto su carta" (Blueprint).

La memoria reale viene costruita solo in due modi:
- **Stack:** Quando dichiari la variabile vera e propria (es. `t_table table;` nel `main`).
- **Heap:** Quando usi `malloc`.

### 2. La Regola d'Oro: Punto (`.`) vs Freccia (`->`)
Per decidere quale operatore usare, analizza **solo l'elemento che hai appena letto a sinistra**.

#### Il Punto (`.`)
Usi il punto quando l'elemento a sinistra è la **Scatola vera e propria**.
```c
t_table table; // table NON ha l'asterisco, è la scatola vera
table.rules.time_to_compile;
```

#### La Freccia (`->`)
Usi la freccia quando l'elemento a sinistra è un **Puntatore** (un "foglietto" con l'indirizzo, identificato dall'asterisco `*`).
```c
t_table *table; // table HA l'asterisco, è solo un puntatore
table->rules.time_to_compile;
```

### 3. Catene di Struct e Puntatori (Il nostro caso pratico)

Perché scriviamo `coder->rules->time_to_compile`?
1. `coder`: È un puntatore (`t_coder *`). Quindi uso la freccia `->`.
2. `rules`: È anch'esso un puntatore dentro al coder (`t_rules *`). Quindi uso di nuovo la freccia `->`.

Perché scriviamo `table->rules.time_to_compile`?
1. `table`: È un puntatore (`t_table *`). Quindi uso la freccia `->`.
2. `rules`: È una scatola fisica dentro alla table (`t_rules rules;` senza asterisco). Quindi uso il punto `.`.
