CC = gcc
CFLAGS = -Wall -Wextra -g -I.
LDFLAGS = -lm

REQUEST_FILES = RequestGenerator/carga_baixa.txt RequestGenerator/carga_media.txt RequestGenerator/carga_alta.txt
REPORT_FILES = ReportManager/estatisticas_first_fit.txt ReportManager/estatisticas_next_fit.txt ReportManager/estatisticas_best_fit.txt
FIRST_FIT_CSV = ReportManager/first_fit_carga_baixa.csv ReportManager/first_fit_carga_media.csv ReportManager/first_fit_carga_alta.csv
NEXT_FIT_CSV = ReportManager/next_fit_carga_baixa.csv ReportManager/next_fit_carga_media.csv ReportManager/next_fit_carga_alta.csv
BEST_FIT_CSV = ReportManager/best_fit_carga_baixa.csv ReportManager/best_fit_carga_media.csv ReportManager/best_fit_carga_alta.csv

# Arquivos .o
ALLOCATED_PROCESSES = MemoryManager/allocated_processes.o
REPORTS = ReportManager/report.o
REQUEST_GENERATOR = RequestGenerator/main.o RequestGenerator/request_generator.o

# `make all` compila todos os 4 executáveis
all: gerar_reqs sim_first_fit sim_next_fit sim_best_fit

gerar_reqs: $(REQUEST_GENERATOR) $(ALLOCATED_PROCESSES)
	$(CC) -o $@ $^ $(LDFLAGS)

sim_first_fit: main.o $(ALLOCATED_PROCESSES) $(REPORTS) MemoryManager/first_fit.o
	$(CC) -o $@ $^ $(LDFLAGS)

sim_next_fit: main.o $(ALLOCATED_PROCESSES) $(REPORTS) MemoryManager/next_fit.o
	$(CC) -o $@ $^ $(LDFLAGS)

sim_best_fit: main.o $(ALLOCATED_PROCESSES) $(REPORTS) MemoryManager/best_fit.o
	$(CC) -o $@ $^ $(LDFLAGS)

run_all: generate_requests run_sims
	@echo "--- Processo completo finalizado. ---"

generate_requests: $(REQUEST_FILES)
	@echo "--- Arquivos de requisição gerados. ---"

$(REQUEST_FILES): gerar_reqs
	@echo "--- Executando gerador de requisições... ---"
	@./gerar_reqs

run_sims: all generate_requests
	@echo "--- Executando 9 simulações (3 algoritmos x 3 cargas) ---"
	@./sim_first_fit first_fit carga_baixa.txt
	@./sim_first_fit first_fit carga_media.txt
	@./sim_first_fit first_fit carga_alta.txt
	
	@./sim_best_fit best_fit carga_baixa.txt
	@./sim_best_fit best_fit carga_media.txt
	@./sim_best_fit best_fit carga_alta.txt
	
	@./sim_next_fit next_fit carga_baixa.txt
	@./sim_next_fit next_fit carga_media.txt
	@./sim_next_fit next_fit carga_alta.txt
	@echo "--- Simulações concluídas. Arquivos de estatísticas gerados. ---"

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

MemoryManager/%.o: MemoryManager/%.c
	$(CC) $(CFLAGS) -c $< -o $@

RequestGenerator/%.o: RequestGenerator/%.c
	$(CC) $(CFLAGS) -c $< -o $@

ReportManager/%.o: ReportManager/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean run_sims generate_requests run_all
clean:
	@echo "--- Limpando arquivos gerados... ---"
	rm -f *.o MemoryManager/*.o RequestGenerator/*.o ReportManager/*.o
	rm -f sim_next_fit sim_best_fit sim_first_fit gerar_reqs
	rm -f $(REQUEST_FILES)
	rm -f $(REPORT_FILES) $(FIRST_FIT_CSV) $(NEXT_FIT_CSV) $(BEST_FIT_CSV)
	@echo "--- Limpeza concluída. ---"