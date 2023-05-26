typedef struct{
  char nome[60];
  int id;
  char* email;
  char* senha;
  char* tipoDeUsuario;
  void* categoriaUsuario;
  int status;
  
}Usuario;


// Tipos de Usuarios

typedef struct{
  struct Usuario usuario;
  int matricula;
  struct Turma turma;
  struct Atividade* listaAtividades;
  struct Submissao* listaSubmissoes;
  struct Preceptor preceptorResponsavel;
  float notasTrimestrais[];
  struct FeedbackGeral* listaFeedbacks;
}Residente;

typedef struct{
  struct Usuario usuario;
  struct Residente* listaResidentesAtuais;
  struct Atividade* listaAtividades;
  struct ProgramaResidencia* listaResidencias;
  struct FeedbackGeral* listaFeedbacks;
}Preceptor;

typedef struct{
  struct Usuario usuario;
  char cargo[];
  struct ProgramaResidencia residencia; residencia;
}Coordenacao;

typedef struct{
  struct Usuario usuario;
  char cargo[];
  struct ProgramaResidencia* listaResidencias;
}Gestao;


// Conjuntos

typedef struct{
  char nomeResidencia[];
  char anoDaTurma[];
  struct Residente* residentes;
  struct Preceptor* listaPreceptores;
  struct Atividade* listasAtividades;
  struct ProgramaResidencia residencia;
  char* criteriosAvaliativos;
}Turma;

typedef struct{
  char nomePrograma[];
  struct Coordenacao* listaCoordenacao;
  struct Turma* listaTurmas
}ProgramaResidencia;


// Funcionalidades da solução

typedef struct{
  char nomeDaAtividade[];
  char descricaoDaAtividade[];
  struct Turma turma;
  char programaDeResidencia[];
  struct Submissao* listaSubmissao;
  char dataDaPostagem;
  char dataDaEntrega;
  int ativa;
}Atividade;

typedef struct{
  struct Residente residente;
  struct Preceptor preceptor;
  struct Atividade atividade;
  float nota;
  char * resposta;
  char feedback[];
  char* status;
}Submissao;

typedef struct{
  struct Residente residente;
  struct Preceptor preceptor;
  char *criterios;
  float nota;
  char *feedback;
  char* contestacao
  char* respostaConstentacao;
  char remetente;
  char destinatário;
  char data[];
  char status(para constentacao);
}FeedbackGeral;

// Funções

void fazerCadastro(){};

struct Usuario* fazerLogin(char* email, char* senha, struct Usuario *usuario){}

void criarCriteriosDeFeedback(struct Turma* turma){}

void criarAtividades(struct Turma* turma){};

void visualizarTurmas(struct Turma* listaTurmas ){};

void visualizarFeedbacks(char* nome);