## Obiective Proiect part. 2
 
| Cerință | Status |
|---|---|
| Templates (cel puțin o clasă/funcție template) | ✅ |
| Design Pattern 1 — Factory | ✅ |
| Design Pattern 2 — Observer | 🔲 în progres |
| Bibliotecă externă — `ftxui` (Terminal UI) | 🔲 |
| Bibliotecă externă — `nlohmann/json` | 🔲 |
| Fără variabile/funcții globale | ✅ |
| Versionare GitHub | ✅ |
 
---
 
## (1) Templates — `MetricHistory<T>`
 
**Problema rezolvată:**
Fiecare metrică colectează valori la intervale de timp, dar până acum nu le stocam. Nu puteam vedea trendul — doar valoarea curentă.
 
**Soluția:**
Clasă template `MetricHistory<T>` care stochează ultimele N valori într-un `std::deque<T>`.
 
**De ce `deque` și nu `vector`?**
`deque` permite `push_back` și `pop_front` ambele în O(1). Când istoricul e plin, scoatem cea mai veche valoare de la cap și adăugăm noua valoare la coadă. Pe `vector`, `pop_front` ar fi O(n) — mută toate elementele.
 
**De ce template și nu `double` direct?**
`MetricHistory<T>` poate stoca orice tip — `double` pentru valori numerice, `std::pair<double, time_point>` pentru valoare + timestamp, `std::string` pentru log-uri. Scriem codul o singură dată, compilatorul generează versiuni pentru fiecare tip folosit.
 
**Implementare:**
```cpp
template <typename T>
class MetricHistory {
private:
    size_t maxSize;
    std::deque<T> history;
public:
    void add(const T& value) {
        if (history.size() == maxSize)
            history.pop_front();
        history.push_back(value);
    }
    T latest() const { return history.back(); }
};
```
 
**Utilizare în proiect:**
- `Metric` conține `MetricHistory<double> history{10}` — ultimele 10 valori
- Fiecare `collect()` apelează `history.add(value)` după ce setează valoarea nouă
---
 
## [2] Design Pattern: Factory — `MetricFactory`
 
**Data:** mai 2026
 
**Problema rezolvată:**
`Menu::addMetric()` conținea un `switch` mare care știa cum să construiască fiecare tip de metrică. Dacă adăugam un tip nou (`GPUMetric`), trebuia modificat `Menu` — și orice alt loc unde se creau metrici.
 
**Soluția — Factory Pattern:**
O clasă separată `MetricFactory` are **singura responsabilitate** de a crea metrici. `Menu` îi pasează tipul și parametrii — Factory știe cum să construiască.
 
**Principiul respectat:** Open/Closed — codul e deschis pentru extensie (adaugi `GPUMetric` doar în Factory), închis pentru modificare (Menu nu se atinge).
 
**Implementare:**
```cpp
class MetricFactory {
public:
    MetricFactory() = delete;  // nu are sens sa instantiezi
    static Metric* create(const std::string& type,
                           const std::string& name,
                           double param1 = 0.0,
                           double param2 = 0.0);
};
```
 
```cpp
Metric* MetricFactory::create(...) {
    if (type == "cpu")     return new CPUMetric(name, param1);
    if (type == "memory")  return new MemoryMetric(name, param1, param2);
    if (type == "disk")    return new DiskMetric(name, param1, param2);
    if (type == "network") return new NetworkMetric(name);
    throw InvalidInputException("Unknown metric type: " + type);
}
```
 
**Înainte vs după în `Menu::addMetric()`:**
```cpp
// Inainte — Menu stia despre fiecare tip
switch(type) {
    case 1: machine->addMetric(new CPUMetric(name, threshold)); break;
    case 2: machine->addMetric(new MemoryMetric(name, max, threshold)); break;
    ...
}
 
// Dupa — Menu delega la Factory
machine->addMetric(MetricFactory::create(type, name, param1, param2));
```
 
---
 
## [3] Design Pattern: Observer — `IMetricObserver` / `AlertObserver`
 
**Data:** mai 2026
 
**Problema rezolvată:**
`Menu::collectMetrics()` verifica manual fiecare metrică cu `dynamic_cast` pentru threshold-uri și genera alerte. `Menu` era responsabil și de UI și de logica de alertare — prea multe responsabilități.
 
**Soluția — Observer Pattern:**
Metrica **notifică automat** observerii când threshold-ul e depășit. Oricine e interesat se înregistrează ca observer.
 
**Structura:**
```
IMetricObserver  ← interfață abstractă (contractul)
    |
AlertObserver    ← implementare: adaugă alerta în vectorul din Menu
```
 
**De ce referință în `AlertObserver`?**
```cpp
std::vector<Alert>& alerts;  // referinta la vectorul din Menu
```
Cu copie, `AlertObserver` ar lucra pe propria lui copie separată — alertele nu ar apărea în `Menu`. Cu referință, ambii pointează spre același vector.
 
**`notifyObservers()` în `Metric`:**
```cpp
void Metric::notifyObservers() {
    ThresholdMetric* tm = dynamic_cast<ThresholdMetric*>(this);
    if (tm != nullptr && tm->isExceeded()) {
        HardwareMetric* hm = dynamic_cast<HardwareMetric*>(this);
        double usagePercent = hm ? hm->getUsagePercent() : -1.0;
        Alert alert(machineName, name, value, tm->getThreshold(),
                    std::chrono::system_clock::now(), unit, usagePercent);
        for (auto obs : observers)
            obs->onThresholdExceeded(alert);
    }
}
```
 
**Apelat la finalul fiecărui `collect()`:**
```cpp
void CPUMetric::collect() {
    value = ...;
    history.add(value);
    notifyObservers();  // notifica automat dupa fiecare colectare
}
```
 
**Avantaj:** dacă vrei să adaugi `LogObserver` sau `JsonObserver` în viitor — zero modificări în `Metric` sau `Menu`. Doar înregistrezi un observer nou.
 
---
 
## [4] Bibliotecă externă — `ftxui` *(în progres)*
 
**Scop:** Transformarea meniului text într-un dashboard interactiv în terminal.
 
---
 
## [5] Bibliotecă externă — `nlohmann/json` *(în progres)*
 
**Scop:** Salvarea și încărcarea configurației mașinilor și istoricului alertelor în fișiere JSON.
 
---
 
## Decizii arhitecturale notabile
 
| Decizie | Motiv |
|---|---|
| `vector<Metric*>` în loc de `vector<Metric>` | `Metric` e abstractă; fără pointer ai object slicing |
| `Machine(const Machine&) = delete` | Copierea unei mașini monitorizate nu are sens; Regula celor 3 |
| `Logger` — static, nu Singleton | Nu are stare; nu are sens să instanțiezi |
| `Menu` — Singleton | Are stare (`machines`, `alerts`); trebuie exact o instanță |
| `virtual public` pe `HardwareMetric` și `ThresholdMetric` | Rezolvă diamantul — o singură copie a lui `Metric` |
| `MetricHistory<T>` — template cu `deque` | `pop_front` O(1); generic pentru orice tip de valoare |
| Factory cu string în loc de enum | Mai extensibil; nu trebuie modificat enum la fiecare tip nou |

**Integrare în Menu:**
- `Menu` conține un `AlertObserver alertObserver{alerts}` — inițializat cu 
  referință la vectorul `alerts`
- La `addMetric()`, după creare: `m->addObserver(&alertObserver)` și 
  `m->setMachineName(machine->getHostname())`
- `collectMetrics()` s-a simplificat — nu mai face `dynamic_cast` manual,
  Observer-ul se ocupă automat de generarea alertelor

**Rezultat:**
- Înainte: Menu verifica manual threshold-uri după fiecare collect
- După: Metric notifică automat observerii — Menu nu știe despre threshold-uri