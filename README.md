# libpq-wrapper
wrapper de base en c++ une class<br> 

travail avec gcc-8 c++11... c++17  <br>

fait avec XUBUNTU / DEBIAN / MANJARO 

il est basé sur libpq C API<br>

ce projet à pour but de simplifier l'accès au travers de libpq mise à disposition par PostGreSql.<br>


projet 2018-03-18  (C) 2018   Copyright 2018 <laroche.jeanpierre@gmail.com><br>

je voudrais remercier...<br>

THANK YOU   MERCI BEAUCOUP<br>


 le site quebecois 					https://h-deb.clg.qc.ca/		pour son travaille de divulgation<br>
 exemple:							https://h-deb.clg.qc.ca/Sujets/Divers--cplusplus/templates_variadiques.html<br>

 https://stackoverflow.com			une mine d'or pour comprendre avec des examples<br>

 https://docs.postgresql.fr/		pour tous les efforts pour la traduction<br>

 
 le site Français wikiversity		https://fr.wikiversity.org/wiki/Langage_C%2B%2B<br>
 le site cppreference				http://en.cppreference.com/w/<br>
									http://fr.cppreference.com/w/<br>
 le site cplusplus					http://www.cplusplus.com/reference/<br>

 le site beaver						https://dbeaver.com/			pour sa simplicité<br>

 le site pgadmin4					https://www.pgadmin.org/		pour tous ses efforts de migrations<br>  

 et tout ceux qui part leur publication sur des sujets pointus mon permis d'avancé.<br>

 ce projet avec ZONED sont intimement lier et conjointement permettent d'approcher un langage de type 4G<br>

 Le sujet qui sera développer après la validation du wrapper<br>


 Le wrapper se base libpq de PostGreSql<br>

 une class libpqwrp.a	           library pq (libpq)  wrp (wrapper)<br>
 ou une class libpqwrp.hpp<br>
 pour contenir les éléments qui permettent de rendre flexible l'ensemble du process.<br>


 pour avoir plusieurs connexions par exemple<br>
 une pour les lock update<br>
 une pour les read<br>
 cela permet de rendre les commits independants etc.....<br>

 de faire des pointeurs et de profiter de ses avantages pour le nettoyage de mémoire.<br>

 le contrôle du clear de PGresult<br>
 la possibilité de faire du istream ou ostream<br>
 la récupération du nom avec traitement switch<br>
 un type template de variadique pour les requêtes<br>
 un autre type stringstream pour les résultats de requête<br>
 possibilté d'avoir plusieurs cursor<br>
 de traiter les double et int<br>

 traitement lock enregistrement   select for update avec serialisation <br>

 traitement des erreurs...

 je souhaite avoir été clair.<br>

 il y a plusieurs approches et de façon de traiter les données<br>
 

 il est certainement possible d'aller beaucoup plus loin,<br>
 soit dans les traitements d'erreurs .....sachez que sous pgsql ECPG on peut aller beaucoup plus loin <br>
 ou insérer des particularitées en relation avec le metier....<br> 
 mais d'après moi cela serait autre chose qu'un wrapper<br>

 ps j'ai laissé en commentaire une ligne pour connaitre les codes retour d'intérroguation .... <br> 
 
 
 N'oubliez pas d'ouvrir un role pour la lecture seulement (read only)  par exemple : <br>
 
 CREATE ROLE Read_Only_User WITH LOGIN PASSWORD 'read' <br>
NOSUPERUSER INHERIT NOCREATEDB NOCREATEROLE NOREPLICATION VALID UNTIL 'infinity'; <br>

GRANT CONNECT ON DATABASE "CGIFCH" TO Read_Only_User; <br>
GRANT USAGE ON SCHEMA public TO Read_Only_User; <br>
GRANT SELECT ON ALL TABLES IN SCHEMA public TO Read_Only_User; <br>
GRANT SELECT ON ALL SEQUENCES IN SCHEMA public TO Read_Only_User;
<br>


Utiliser la Lib libpqwrp.a ex: Tiostream ou  #include <libpqwrp.hpp>   ex:Tclasse (Encours d'élaboration)

	slc.begin();
	requete = slc.prepare( \
	"SELECT " \
	"cl.column_name,cl.ORDINAL_POSITION,cl.DATA_TYPE,cl.CHARACTER_MAXIMUM_LENGTH,cl.NUMERIC_PRECISION,cl.NUMERIC_SCALE " \
	",(select pg_catalog.col_description(oid,cl.ordinal_position::int) from pg_catalog.pg_class c where c.relname=cl.table_name) as column_comment " \
	"FROM information_schema.columns cl " \
	"WHERE cl.table_catalog='?'  and cl.table_name='?' " \
	"  order by 2 ; " ,(char*) gtk_entry_get_text(eDATABASE) ,(char*) gtk_label_get_text(eTABLE) );

	slc.begin();
	slc.opensql(requete, cursorName); 	 
	if ( !slc.errorSQL ) do
	{
		if ( ! slc.fetchEOF )
		{
			

			sqlx = slc.result();

			sqlx>>column_name>>column_ordre>>column_type>>column_length>>column_precision>>column_scale>>column_comment;		
			std::cout<<column_name<<"  "<<column_ordre<<"  "<<column_type<<" :"<<column_length<<":  "<<column_precision<<","<<column_scale<<" >>>> "<<column_comment<<std::endl;

			slc.fetchsql(cursorName);
		}
 	}while  ( !slc.fetchEOF ) ;	

	slc.end();


	or

	slc.begin();
	slc.fetchAll(requete, cursorName);
	printMsg("Generator_Field"); 
    for (int row = 0; row < slc.countrow() && slc.fetchEOF ==false ; row++)
    {
		for (int nf = 0;nf < slc.countfield(); nf++)
			{ 

				switch (HashStringToInt(slc.cfield(nf)))
				{
					case HashStringToInt(NAMEOF(column_name)):			column_name			=	slc.fetch( row, nf);	break; //exemple avec le Nom de la Variable
					case HashStringToInt("ordinal_position"):			column_ordre		=	slc.fetchInt( row, nf);	break;
					case HashStringToInt("data_type"):					column_type			=	slc.fetch( row, nf);	break;
					case HashStringToInt("character_maximum_length"):	column_length		=	slc.fetchInt( row, nf);	break;
					case HashStringToInt("numeric_precision"):			column_precision	=	slc.fetchInt( row, nf);	break;
					case HashStringToInt("numeric_scale"):				column_scale		=	slc.fetchInt( row, nf);	break;
					case HashStringToInt(NAMEOF(column_comment)):		column_comment		=	slc.fetch( row, nf);	break;
				}
			}
			std::cout<<column_name<<"  "<<column_ordre<<"  "<<column_type<<" :"<<column_length<<":  "<<column_precision<<","<<column_scale<<" >>>> "<<column_comment<<std::endl;
	}

<br>
un exemple de génération de source pour des requêtes
<br>
