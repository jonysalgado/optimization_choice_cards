#include "utils.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>   

using namespace std;

vector<vector<int>> matrix_from_csv(string file);

tuple<vector<int>, vector<int>, vector<int>, vector<int>, vector<int>> 
return_id_by_categories(vector<vector<int>> df);

vector<vector<int>> 
cartesian_product(vector<int> r1, vector<int> r2, vector<int> r3, vector<int> r4);

tuple<vector<int>, vector<vector<int>>, vector<int>>
get_choice_cards(vector<int> four_cards, vector<int> deck);

tuple<int, int, vector<int>> 
params_cards(vector<vector<int>> df, vector<int> E);


int main(){
    vector<vector<int>> df = matrix_from_csv("deck_rodada3_c.csv");
    // Variables
    int N_CARTAS = 8;
    int O_rodada = 10148;
    double C_max = 10.0;
    double P_max = 16.0;
    double N_pitch = 0.8;

    // Pontos referentes às expectativas do mercado
    int I_max = 8;
    int Q_max = 20;
    int V_max = 18;
    int S_max = 15;
    int R_max = 11;
    int E_max = 14;
    vector<int> pts_max{I_max, Q_max, V_max, S_max, R_max, E_max};
    // Valor  mínimo para V_bruto considerado [GED$]
    double V_bruto_min = 8200.0;

    tuple<vector<int>, vector<int>, vector<int>, vector<int>, vector<int>> Es;
    Es = return_id_by_categories(df);
    vector<int> Ep = get<0>(Es);
    vector<int> Eo = get<1>(Es);
    vector<int> El = get<2>(Es);
    vector<int> Ec = get<3>(Es);
    vector<int> deck = get<4>(Es);

    vector<vector<int>> product = cartesian_product(Ep, Eo, El, Ec);
    vector<vector<int>> best_choices;
    vector<int> best_results;

    // Product size: 72960 , Ex size: 595665

    for(int i = 0; i < product.size(); i++) {
        tuple<vector<int>, vector<vector<int>>, vector<int>> choices;
        choices = get_choice_cards(product[i], deck);
        vector<int> four_cards = get<0>(choices);
        vector<vector<int>> Ex = get<1>(choices);
        vector<int> X_s = get<2>(choices);

        for(int j = 0; j < Ex.size(); j++){
            vector<int> X_s_s(X_s.begin(), X_s.end());
            vector<int> E_s_s(four_cards.begin(), four_cards.end());

            E_s_s.insert(E_s_s.end(), Ex[j].begin(), Ex[j].end());
            // Simulação numérica
            tuple<int, int, vector<int>> params = params_cards(df, E_s_s);
            int O_cards = get<0>(params);
            int reduc_custos = get<1>(params);
            vector<int> points = get<2>(params);
            int O_producao = O_rodada - O_cards;
            double C_producao = C_max * (1 - 0.02 * N_pitch * (double)reduc_custos);
            double Q = (double)O_producao / C_producao;
            // cout << Q << endl;
            vector<double> efetivo(points.size(), 0.0);
            for(int k=0; k < pts_max.size(); k++){
                double vantagem = 1.0;
                if(points[k] > pts_max[k]){
                    vantagem = 1.0;
                }
                else{
                    vantagem = (double)points[k]/ (double)pts_max[k];
                }
                efetivo[k] = vantagem * (double)pts_max[k];
            }  

            double sum_efetivo = 0;
            double sum_pts_max = 0;
            for(int k=0; k < pts_max.size(); k++){
                sum_efetivo = sum_efetivo + efetivo[k];
                sum_pts_max = sum_pts_max + (double)pts_max[k];
            }
            double n_atrativ = sum_efetivo/sum_pts_max;
            // cout << n_atrativ << endl;
            double P = N_pitch * n_atrativ * P_max;
            double V_bruto = P * Q;

            // cout << V_bruto << endl;
            if(V_bruto > V_bruto_min){
                best_choices.push_back(E_s_s);
                best_results.push_back(V_bruto);
                cout << V_bruto << " - Product: " << (double)i * 100/(double)product.size() << "% - Ex:" << (double)j * 100/(double)Ex.size() << endl;
            }

        }
        write_csv(best_choices, best_results, "best_choices.csv");

    }
    write_csv(best_choices, best_results, "best_choices.csv");
        
    return 0;
}

vector<vector<int>> matrix_from_csv(string file){
    vector<pair<string, vector<int>>> dataset = read_csv(file);
    int n_lines = dataset.at(0).second.size();
    int columns = dataset.size();
    vector<vector<int>> values(n_lines, vector<int> (columns, 0));

    for(int i = 0; i < n_lines; i++){   
        for(int j = 0; j < columns; j++){
            values[i][j] = dataset[j].second[i];
        }
    }

    return values;
}

tuple<vector<int>, vector<int>, vector<int>, vector<int>, vector<int>> 
return_id_by_categories(vector<vector<int>> df){
    int n_lines =df.size();
    vector<int> Ep, Eo, El, Ec, Es;
    for(int i = 0; i < n_lines; i++){
        Es.push_back(df[i][0]);
        if(df[i][1] == 0){
           Ep.push_back(df[i][0]);
        }
        if(df[i][1] == 1){
           Eo.push_back(df[i][0]);
        }
        if(df[i][1] == 2){
           El.push_back(df[i][0]);
        }
        if(df[i][1] == 3){
           Ec.push_back(df[i][0]);
        }
    } 

    return make_tuple(Ep, Eo, El, Ec, Es);

}

vector<vector<int>> 
cartesian_product(vector<int> r1, vector<int> r2, vector<int> r3, vector<int> r4) {

    
    vector<vector<int>> result;
    for (auto p = r1.begin();p != r1.end(); p++) {
        for (auto o = r2.begin();o != r2.end(); o++) {
            for (auto l = r3.begin();l != r3.end(); l++) {
                for (auto c = r4.begin();c != r4.end(); c++) {
                    vector<int> line{*p, *o, *l, *c};
                    result.push_back(line);
                }
            }
        }
    }
    return result;
}

tuple<vector<int>, vector<vector<int>>, vector<int>>
get_choice_cards(vector<int> four_cards, vector<int> deck){
    vector<int> choice(four_cards.begin(), four_cards.end());
    vector<int> X_s;
    int n_cards = deck.size();
    bool isRepetitive = false;
    for(int i = 0; i < n_cards; i++){
        bool isRepetitive = false;
        for(int j = 0; j < choice.size() && !isRepetitive; j++)
            if(deck[i] == choice[j])
                isRepetitive = true;
        
        if(!isRepetitive)
            X_s.push_back(deck[i]);
    }
    // cout << X_s.size() << endl;
    vector<vector<int>> Ex;
    for(int i1 = 0; i1 < X_s.size(); i1++){
        for(int i2 = i1+1; i2 < X_s.size(); i2++){
            for(int i3 = i2+1; i3 < X_s.size(); i3++){
                for(int i4 = i3+1; i4 < X_s.size(); i4++){
                    vector<int> line{X_s[i1], X_s[i2], X_s[i3], X_s[i4]};
                    Ex.push_back(line);
                }
            }
        }
    }
    // cout << "ok";
    return make_tuple(four_cards, Ex, X_s);
}

tuple<int, int, vector<int>> 
params_cards(vector<vector<int>> df, vector<int> E){

    int O_cards = 0;
    int reduc_custos = 0;
    int index = 0;
    vector<int> points(6, 0);
    for(int i = 0; i < E.size(); i++){
        index = 0;
        if(E[i] < 100){
            index = E[i] - 1;
        }
        else if(E[i] < 200){
            index = E[i] - 81;
        }
        else if(E[i] < 300){
            index = E[i] - 162;
        }
        else{
            index = E[i] - 246;
        }
        O_cards = O_cards + df[index][9];
        reduc_custos = reduc_custos + df[index][8];
        for(int j = 0; j < 6; j++){
            points[j] = points[j] + df[index][j + 2];
        }
    }

    return make_tuple(O_cards, reduc_custos, points);
}