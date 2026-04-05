# Argus 👁️
A system monitor application  with analog style display for Homelabs, built in C++.
> This project was developed as part of the course "Object-Oriented Programming", under the supervision of [Prof. PhD. Andrei PAUN](https://scholar.google.com/citations?user=wWNnd94AAAAJ&hl=en), at the Department for Mathematics and Computer Science, University of Bucharest.

Argus monitors multiple machines by collecting and tracking hardware and network metrics (CPU, Memory, Disk, Network). It detects anomalies and threshold violations and generated alerts automatically.

## Class Hierarchy
##### Legenda:
| Simbol | Inseamna |
|---|---|
| `-` | `private` |
| `#` | `protected` |
| `+` | `public` |
| `$` | `static` |
| `*` | `pur virtual/abstract` |

```mermaid
classDiagram
    class Metric{
        <<abstract>>
        #string name
        #double value
        #string unit
        #time_point lastCollected
        +collect()* void
        +getName() string
        +getValue() double
        +getUnit() string
        +operator<<()
    }

    class HardwareMetric {
        <<abstract>>
        #double maxValue
        +getUsagePercent() double
        +collect()* void
    }

    class ThresholdMetric {
        <<abstract>>
        #double threshold
        +isExceeded() bool
        +getThreshold() double
        +collect()* void
    }

    class CPUMetric {
        +CPUMetric(name, threshold)
        +collect() void
    }

    class MemoryMetric {
        +MemoryMetric(name, maxValue, threshold)
        +collect() void
        +isExceeded() bool
    }

    class DiskMetric {
        +DiskMetric(name, maxValue, threshold)
        +collect() void
        +isExceeded() bool
    }

    class NetworkMetric {
        +NetworkMetric(name)
        +collect() void
    }

    class Machine {
        -string hostname
        -string ipAddress
        -vector~Metric*~ metrics
        +addMetric(Metric*)
        +collectAll()
        +displayAll()
        +getMetrics()
    }

    class Alert {
        -string machineName
        -string metricName
        -double value
        -double threshold
        -double usagePercent
        -string unit
        -time_point timestamp
        +displayAlert()
    }

    class Logger {
        <<static>>
        +init()$
        +info()$
        +warning()$
        +error()$
        +close()$
    }

    class Menu {
        <<singleton>>
        -vector~Machine*~ machines
        -vector~Alert~ alerts
        +getInstance()$ Menu&
        +run()
    }

    Metric <|-- HardwareMetric : virtual public
    Metric <|-- ThresholdMetric : virtual public
    Metric <|-- NetworkMetric

    HardwareMetric <|-- CPUMetric
    HardwareMetric <|-- MemoryMetric
    HardwareMetric <|-- DiskMetric

    ThresholdMetric <|-- CPUMetric
    ThresholdMetric <|-- CPUMetric
    ThresholdMetric <|-- CPUMetric

    Machine o-- Metric : vector~Metric*~
    Menu o-- Machine : vector~Machine*~
    Menu o-- Alert : vector~Alert~
```

### Diamond Inheritance

`CPUMetric`, `MemoryMetric` and `DiskMetric` each inherit from both `HardwareMetric` and `ThresholdMetric`, both of which inherit from `Metric` using `virtual public`. This ensured a single shared instance of `Metric` per object.

```mermaid
graph TD
    Metric
    HardwareMetric
    ThresholdMetric
    CPU
    Memory
    Disk

    Metric --> HardwareMetric
    Metric --> ThresholdMetric
    HardwareMetric --> CPU
    HardwareMetric --> Memory
    HardwareMetric --> Disk
    ThresholdMetric --> CPU
    ThresholdMetric --> Memory
    ThresholdMetric --> Disk
```

### Cerinte proiect

| Requirement | Implementation |
|---|---|
| Minim 10 clase | 12 clase: `Metric`, `HardwareMetric`, `ThresholdMetric`, `CPUMetric`, `MemoryMetric`, `DiskMetric`, `NetworkMetric`, `Machine`, `Alert`, `Logger`, `Menu`, `Exceptions` |
| Ierarhie de mostenire (6+ classes) | `Metric` → `HardwareMetric` → `CPUMetric/MemoryMetric/DiskMetric` (3 niveluri) |
| Mostenire diamant | `CPUMetric`, `MemoryMetric`, `DiskMetric` cu `virtual public` |
| Metode si membri static | `Logger` — (`init`, `info`, `warning`, `error`, `close`) |
| Encapsulare | `private`/`protected`/`public` folosite; `value` e `protected` pentru suprascriere in subclase |
| Regula celor Trei | `Machine` are `destructor` custom. Am decis ca copierea nu are sens pentru o masina monitorizata, deci am interzis explicit `constructorul de copiere` și `operator=` |
| `std::vector` | `Machine` contine `vector<Metric*>`; `Menu` contine `vector<Machine*>` si `vector<Alert>` |
| Polimorfism si clase abstracte | `Metric` e abstracta (`collect() = 0`); `operator<<` prin virtual `afisare()` |
| Exceptii custom | `ArgusException`, `InvalidMetricException`, `ThresholdExceededException`, `MachineNotFoundException`, `InvalidInputException` |
| Meniu | `add/remove machines`, `add metrics`, `collect`, `display`, `alerts` |
| Singleton | `Menu::getInstance()` — instanta unica, copie stearsa |
| `.h` / `.cpp` separate | Toate clasele sunt distribuite in `include/` si `src/` |
| README | Ce priveste |
 
---