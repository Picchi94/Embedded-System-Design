Nella cartella results è possibile consultare:

1) Il pdf schematic_before_synthesis.pdf che contiene lo schema del dispositivo prima della sintesi

2) Il pdf schematic_after_synthesis.pdf che contiene lo schema del dispositivo dopo la sintesi

3) Il report dettagliato della sintesi logica. Il file si chiama utilization.txt

4) Il report dettagliato della High Level Synthesis. Il file si chiama xtea_HLS_report.html

Sono presenti inoltre i codici generati dalla High Level Synthesis dello xtea.cpp in SystemC, Verilog e VHDL.

Nella cartella VHDL_behevioral_implementation sono presenti il file xtea.vhdl contenente l'implementazione in vhdl dello xtea, il file stimuli.do e il file vsim.wlf contenente la simulazione completa tracciata.

Per compilare il file .vhdl è necessario aprire Modelsim e digitare i comandi:
vlib work per creare la directory di lavoro

vcom -93 xtea.vhdl per compilare il modello

vsim work.xtea per aprire il simulatore e simulare il tutto

do stimuli.do per automatizzare l'impostazione della simulazione

Nella cartella HSL_model è presente il file xtea.cpp con il quale è stata effettuata l'High Level Synthesis
