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
MEMORY = MemoryManager/memory.o MemoryManager/alloc_algorithm.o
REPORTS = ReportManager/report.o
REQUEST_GENERATOR = RequestGenerator/main.o RequestGenerator/request_generator.o

# `make all` compila todos os 4 executáveis
all: gerar_reqs sim

gerar_reqs: $(REQUEST_GENERATOR) $(ALLOCATED_PROCESSES)
	$(CC) -o $@ $^ $(LDFLAGS)

sim: main.o $(ALLOCATED_PROCESSES) $(MEMORY) $(REPORTS)
	$(CC) -o $@ $^ $(LDFLAGS)

run_all: generate_requests run_sim
	@echo "--- Processo completo finalizado. ---"

generate_requests: $(REQUEST_FILES)
	@echo "--- Arquivos de requisição gerados. ---"

$(REQUEST_FILES): gerar_reqs
	@echo "--- Executando gerador de requisições... ---"
	@./gerar_reqs

run_sim: all generate_requests
	@echo "--- Executando 9 simulações (3 algoritmos x 3 cargas) ---"
	@./sim first_fit carga_baixa.txt
	@./sim first_fit carga_media.txt
	@./sim first_fit carga_alta.txt
	
	@./sim best_fit carga_baixa.txt
	@./sim best_fit carga_media.txt
	@./sim best_fit carga_alta.txt
	
	@./sim next_fit carga_baixa.txt
	@./sim next_fit carga_media.txt
	@./sim next_fit carga_alta.txt
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
	# Remove .o de todas as pastas
	rm -f *.o MemoryManager/*.o RequestGenerator/*.o ReportManager/*.o
	# Remove os executáveis
	rm -f sim gerar_reqs
	# Remove os arquivos de carga gerados
	rm -f $(REQUEST_FILES)
	# Remove os arquivos de relatório gerados
	rm -f $(REPORT_FILES) $(FIRST_FIT_CSV) $(NEXT_FIT_CSV) $(BEST_FIT_CSV)
	@echo "--- Limpeza concluída. ---"