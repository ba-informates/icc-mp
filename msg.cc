#include <iostream>
#include <vector>

using namespace std;

// represents a chat between two clients
struct Chat {
  string source_client_name;
  string target_client_name;
  time_t opened_at;
};

struct Client {
  string name;
  string genre;
  string username;
};

struct System {
  vector<Client> clients;
  vector<Chat> chats;
};

// validate genre
bool is_valid_genre(char genre) {
  return genre == 'F' || genre == 'M' || genre == 'X';
}

void add(System& network, string client_name, char client_genre, string client_username) {

  // check whether there is a client with the same username
  for (size_t i = 0; i < network.clients.size(); ++i) {
    if (network.clients[i].username == client_username) {
      cout << "ERREUR: pseudo \"" << client_username << "\" déjà présent" << endl;
      return;
    }
  }

  // check whether genre is valid
  if (!is_valid_genre(client_genre)) {
    cout << "ERREUR: genre '" << client_genre << "' non valide" << endl;
    return;
  }
  
  if (client_genre == 'F') {
    cout << "Nouvelle cliente : ";
  } else if (client_genre == 'M') {
    cout << "Nouveau client : ";
  } else {
    cout << "Nouveau/Nouvelle client(e) : ";
  }

  cout << client_name + " - " + client_username << endl;

  Client new_client;
  new_client.name = client_name;
  new_client.genre = client_genre;
  new_client.username = client_username;

  network.clients.push_back(new_client);

}


void chat(System& network, size_t client_in_idx, string client_out_username) {

  // find client in details
  Client client_in = network.clients[client_in_idx];

  cout << client_in.name + " contacte " + client_out_username << endl;

  // find client out details
  Client client_out;
  bool client_out_found = false;
  for (int i = 0; i < network.clients.size(); ++i) {
    if (network.clients[i].username == client_out_username) {
      client_out = network.clients[i];
      client_out_found = true;
      break;
    }
  }

  if (!client_out_found) {
    cout << " ==> ERREUR : PAS DANS LE RESEAU" << endl;
    return;
  }

  // now find whether there is a chat between the two clients

  bool chat_found = false;
  for (int i = 0; i < network.chats.size(); ++i) {
    Chat chat = network.chats[i];
    if (
        // check whether there is a in to out chat
        (chat.source_client_name == client_in.username && chat.target_client_name == client_out.username)
        // check whether there is a out to in chat
        || (chat.source_client_name == client_out.username && chat.target_client_name == client_in.username)
      ) {
      chat_found = true;
      break;
    }
  }

  if (chat_found) {
    cout << " ==> ERREUR : COMMUNICATION DEJA EXISTANTE" << endl;
    return;
  }

  // now create a new chat
  Chat new_chat;
  new_chat.source_client_name = client_in.username;
  new_chat.target_client_name = client_out.username;
  new_chat.opened_at = time(NULL);

  network.chats.push_back(new_chat);

  cout << " ==> OK (" + client_out.name + ")" << endl;

}

vector<Chat> get_chats (System network, string client_name) {
  vector<Chat> chats;
  for (size_t i = 0; i < network.chats.size(); ++i) {
    Chat chat = network.chats[i];
    if (
      chat.source_client_name == client_name
      || chat.target_client_name == client_name
    ) {
      chats.push_back(chat);
    }
  }
  return chats;
}

Client get_client_from_username (System network, string username) {
  for (size_t i = 0; i < network.clients.size(); ++i) {
    Client client = network.clients[i];
    if (client.username == username) {
      return client;
    }
  }
  return Client();
}

void state(System network) {

  for (size_t i = 0; i < network.clients.size(); ++i) {
    Client client = network.clients[i];
    cout << "Discussions de " + client.name + " :" << endl;
    vector<Chat> chats = get_chats(network, client.username);

    for (size_t j = 0; j < chats.size(); ++j) {
      Chat chat = chats[j];
      bool is_in = chat.source_client_name == client.username;
      Client matching_client = get_client_from_username(network, is_in ? chat.target_client_name : chat.source_client_name);
      cout << (is_in ? " -> " : "<- ") + matching_client.username + " (" + matching_client.name + ")" << endl;
    }
  }

}


// ==============    main    ==============

int main()
{
  System network;

  add(network, "Paul Hice"        , 'M', "interPaul");
  add(network, "Sylva Froid"      , 'F', "sylv21"   );
  add(network, "Melusine Titgoute", 'F', "kerrig@n" );
  add(network, "Bob Razowsky"     , 'X', "sylv21"   ); // échec
  add(network, "Bob Razowsky"     , 'Y', "bobichon" ); // échec
  add(network, "Bob Razowsky"     , 'X', "bobichon" );

  cout << endl;
  chat(network, 0, "serv@l"   );
  chat(network, 0, "sylv21"   );
  chat(network, 0, "kerrig@n" );
  chat(network, 0, "kerrig@n" );
                
  chat(network, 1, "interPaul");
                
  chat(network, 2, "sylv21"   );
  chat(network, 2, "bobichon" );
  
  cout << endl;
  state(network);

  return 0;
}

/* ==== Quelques messages fournis à toutes fins utiles :

    cout << "ERREUR: pseudo \"" << pseudo << "\" déjà présent" << endl;
    cout << "Nouveau client : ";
    cout << "Nouvelle cliente : ";
    cout << "Nouveau/Nouvelle client(e) : ";
    cout << "ERREUR: genre '" << gender << "' non valide" << endl;
    cout << " ==> ERREUR : PAS DANS LE RESEAU" << endl;
    cout << " ==> ERREUR : APPEL DEJA FAIT" << endl;
    cout << " ==> ERREUR : COMMUNICATION DEJA EXISTANTE" << endl;

*/
