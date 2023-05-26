#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqlite3/sqlite3.h"


#ifndef UTILS_H_
#define UTILS_H_

typedef struct {
  char nome[60];
  int id;
  char *email;
  char *senha;
  char *tipoDeUsuario;
  void *categoriaUsuario;
  int status;

} Usuario;

// Tipos de Usuarios

/*

typedef struct {
  Usuario usuario;
  int matricula;
  Turma turma;
  Atividade *listaAtividades;
  Submissao *listaSubmissoes;
  Preceptor preceptorResponsavel;
  float notasTrimestrais[];
  FeedbackGeral *listaFeedbacks;
} Residente;

typedef struct {
  Usuario usuario;
  Residente *listaResidentesAtuais;
  Atividade *listaAtividades;
  ProgramaResidencia *listaResidencias;
  FeedbackGeral *listaFeedbacks;
} Preceptor;

typedef struct {
  Usuario usuario;
  char cargo[];
  ProgramaResidencia residencia;
  residencia;
} Coordenacao;

typedef struct {
  Usuario usuario;
  char cargo[];
  ProgramaResidencia *listaResidencias;
} Gestao;

// Conjuntos

typedef struct{
  char nomePrograma[];
  Coordenacao* listaCoordenacao;
  Turma* listaTurmas
}ProgramaResidencia;


typedef struct{
  char nomeResidencia[];
  char anoDaTurma[];
  Residente *residentes;
  Preceptor *listaPreceptores;
  Atividade *listasAtividades;
  ProgramaResidencia residencia;
  char *criteriosAvaliativos;
} Turma;


// Funcionalidades da solução

typedef struct {
  char nomeDaAtividade[];
  char descricaoDaAtividade[];
  Turma turma;
  char programaDeResidencia[];
  Submissao *listaSubmissao;
  char dataDaPostagem;
  char dataDaEntrega;
  int ativa;
} Atividade;

typedef struct {
  Residente residente;
  Preceptor preceptor;
  Atividade atividade;
  float nota;
  char *resposta;
  char feedback[];
  char *status;
} Submissao;

typedef struct {
  Preceptor preceptor;
  char *criterios;
  float nota;
  char *feedback;
  char *contestacao char *respostaConstentacao;
  char remetente;
  char destinatário;
  char data[];
  char status(para constentacao);
}FeedbackGeral;
*/

// Funções

void fazerCadastro();

Usuario *fazerLogin(char *email, char *senha);

/*
void criarCriteriosDeFeedback(Turma *turma);

void criarAtividades(Turma *turma);

void visualizarTurmas(Turma *listaTurmas);

void visualizarFeedbacks(char *nome);

void visualizarAtividades(Atividade *listaAtividades);

void submeterAtividade(Residente *residente, Submissao *submissao);

void visualizarSubmissao(Submissao *submissao);

void contestarFeedback(Usuario *usuario, feedbackGeral feedbackGeral);

void visualizarUsuario(Usuario *usuario);

void avaliarAtividade(Preceptor *preceptor, Submissao *submissao);

void fazerAvaliacaoTrimestral(Usuario *usuario, float notaTrimestral);

void fazerFeedback(Preceptor *preceptor, Residente *residente, Usuario usuario);

void cadastrarTurma(ProgramaResidencia* residencia);
*/

#endif // UTILS_H_