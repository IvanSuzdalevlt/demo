	/**
	 * Skriptas skirtas lentų skirstymui i puslapius pagal horizontale
	 * Skriptas ieško visose lentose reikiamų data tagų, pagal kuriuos
	 *  nusprendžia kad lentoms reikia taikyti suspaudimą.
	 * Kiekvienam puslapyje yra tam tikras kiekis stulpelių.
	 * Skriptas prisirisa prie lentų pagal sekancius data tag'us:
	 * 
	 * data-table-id turi būtį priligintas unikaliam ID
	 * 		PVZ data-table-id = "LENT_NR1"
	 * 		Šis tagas gali būti lygus bet kokiam char ženklų deriniui - skaičiai, raidės, ženklai,
	 * 		tačiau kiekviena lenta turi turėti unikalų ID tag'ą
	 * 		
	 * data-col-id turi buti įstatomi į tuos <td><th> HTML elementus, kurie bus slepiami
	 * 		Jei šis tag'as įstatomas į <td> tagus, reiškia atitinkami aukščiau stovintys <th> tagai taipogi
	 * 		turi būti paslėpti, reikia į juos irgi reikia įrašyti data-col-id tag'ą
	 * 		data-col-id tagas turi būti prilyginamas tam tikram skaičiui.
	 * 		Pagal šių skaičių seką viks visų stulpelių perrinkimas
	 * 		Skaičiai turi eiti nuosekliai 0..1..2..3..4 ir t.t.
	 * 		Galima žymėti po vieną stulpeliu vienu data-col-id, o galima žimėti ištisus stulpelių
	 * 		blokus vienu unikalių data-col-id. Tokiu atveju slėpimas bus jungiamas pagal data-col-id 
	 * 		visai stulpelių grupei.
	 * 		PVZ data-col-id prilyginamas 2: data-col-id = "2"
	 * 
	 * data-hide-is-on užduoda išankstinė lentos slėpimo įjungimą. 
	 * 		Gali būti prilyginamas 1 arba 0. Jei šis tagas nustatytas į 0, įdedamas tik lentos 
	 * 		suspaudimo įjungimo linkas, jei į 1, įdedamas lentos suspaudimo linkas ir iškarto paleidžiamas
	 * 		lentos suspaudimas. Tačiau lentos suspaudimas atima daug resursų, todėl nepatartina
	 * 		įjungti pradinio lentų suspaudimo daugeliui lentu vienu metu
	 * 		PVZ data-hide-is-on = "1"
	 * 
	 * data-count-columns užduoda blokų, kurie sunumeruoti data-col-id taguose, skaičių. Šis skaičius turi 
	 * 		sutapti su  data-col-id numeracija, t.y. prasidėti nuo 0 ir pasibaigti paskutyniu data-col-id  numerių.
	 * 		PVZ data-count-columns = "190"
	 * 
	 * data-count-columns nustato blokų(arba stulpelių) kiekį, kuris bus rodomas vienu metu, kai įjungtas lentos suspaudimas
	 * 		PVZ data-count-columns = "20"
	 * 
	 *  PVZ lentos inicijavimas:
	 *  	
	 * 		echo "<table data-table-id = ".$lenteles_id." data-show-columns = '30' 
	 * 			data-hide-is-on = '0' data-count-columns = ".$count_columns." class='items table table-striped table-condensed'>"; 
	 * 
	 * 		Ir į <th> bei <td> tag'us įdedamas data-col-id:
	 * 
	 * 		echo "<th  data-col-id = ".$col_counter.">";
	 * 		echo "<td  data-col-id = ".$col_counter.">";
	 */
	$( document ).ready(function() {
		
		//Kuriamas elementu kiekis
		var table_array_objects = new Object();
		
		$('table[data-table-id]').each(function(index, value)
			{	
				//elementai kesuojamas kintamajame, kad nevykdyti pajieskos DOM medyje
				var $table_id = $(this).data('table-id');
				var $show_columns = $(this).data('show-columns');
				var $count_columns = $(this).data('count-columns');
				var $hide_is_on = $(this).data('hide-is-on');
				
				/**
				 * Table Object container
				 * Konstruktorius, kuris kuria kiekvienai lentelei jos objekta, kuriame
				 * bus lentele valdantys metodai
				 */
				table_array_objects[$table_id] = {
					//Naujam lenteles objektui priskiriamas ID
					table_id:$table_id,
					//Naujam lenteles objektui priskiriamas rodomu stulpeliu kiekis
					show_columns:$show_columns,
					//Naujam lenteles objektui priskiriamas bendras visu stulpeliu kiekis
					count_columns:$count_columns,
					
					//Inicijuojamas stulpelio, nuo kuriuo prasides atvaizdavimas, numeris
					begin_col: 0,
					//Inicijuojamas stulpelio, ties kuriuo pasibaigs atvaizdavimas numeris
					end_col: 0,
					//lenteles stulpeliu slepimo ijungimo leidimas
					hide_is_on: $hide_is_on,
					
					//Rodykles i kaire nuoroda, kol kas inicijuojama kaip tuscia reiksme
					left_arrow: null,
					//Rodykles i desine nuoroda, kol kas inicijuojama kaip tuscia reiksme
					right_arrow: null,
					
					/**
					 * Metodas ijungia siuo metu aktualios lentos visus stulpelius
					 */
					cols_show: function(){
						
						$('table[data-table-id="'+this.table_id+'"]').find('[data-col-id]').show();
						
					},
					
					/**
					 * Metodas slepia stulpelius, kurie nera tarp begin_col ir end_col stulpeliu
					 */
					cols_hide: function(){
						//Slepiamos visos lenteles
						$('table[data-table-id="'+this.table_id+'"]').find('[data-col-id]').hide();
												
						//Skaiciuojami stulpeliai nuo kurio iki kurio viks rodymas
						if((this.begin_col+this.show_columns)<=this.count_columns) this.end_col = this.begin_col+this.show_columns
						else this.end_col = this.count_columns;
						
						var col_nr = 0;
						
						//Viena karta gaunamas linkas i dabartines lentos objekta, kuris bus naudojamas daug kartu
						var current_table_link = $('table[data-table-id="'+this.table_id+'"]');
						
						//Perrenkami stulpeliai kuriuose viks rodymas nuo pradinio iki pabaigos ir ijungiamas tu stulpeliu atvaizdavimas
						for (col_nr=this.begin_col;col_nr < this.end_col;col_nr++ ) {
							
							//Jungiamas stulpeliu atvaizdavimas
							current_table_link.find('[data-col-id = "'+col_nr+'"]').show();
								
						}
						
						//Is abeju pusiu nuo rodomu stulpeliu bloko pridedamas stulpelis, kuris veiks kaip mygtukas
						//Pridejimas viksta, jei kintamieji dar nebuvo inicijuoti, reiskia rodykliu stulpeliai dar nebuvo sukurti
						if(this.right_arrow == null) current_table_link.
														find('[data-col-id = "'+(this.count_columns-1)+'"]')
														.after('<td data-table-id = '+this.table_id+' class = "right_arrow" data-arrow = "right" width="30"><a href="javascript:;"  class = "right_arrow_link"> > </a></td>');
						if(this.left_arrow == null) current_table_link
														.find('[data-col-id = "0"]')
														.before('<td data-table-id = '+this.table_id+' class = "left_arrow" width="30"><a href="javascript:;" class = "left_arrow_link" > < </a></td>');
						
						//Kuriami linkai i rodykliu stulpelius
						this.left_arrow  = current_table_link.find('.left_arrow');
						this.right_arrow = current_table_link.find('.right_arrow');
										
						
						//Skaiciujami puslapiu skaitliukai. Kadangi puslapiai numeruojami nuo 0, pridedamas 1 puslapiu kiekiui nustatyti
						var psl_count = parseInt(this.count_columns/this.show_columns,0)+1;
						var current_psl = parseInt(this.begin_col*psl_count/this.count_columns)+1;
						//Generuojamas puslapiu skaitliukas, rodantis si puslapi bei kiek viso puslapiu yra
						var counter = current_psl + '/' + psl_count;
												
						//Generuojamos rodykles ir skaitliukas
						current_table_link.find('.left_arrow_link').text(' < '+counter);
						current_table_link.find('.right_arrow_link').text(' > '+counter);
						
						
						
					},
					
					/**
					 * Poslinkio kairen metodas pastumia stulpeliu skaitliukus
					 */
					cols_left_slide: function() {
						
						//Vykdomas stulpeliu postumis ir tikrinama ar stulpeliu blokas neisejo uz rodymo rybu
						this.begin_col = this.begin_col - this.show_columns;
						//Jei maziau nei 0 , prilyginama 0
						if(this.begin_col < 0) this.begin_col = 0;
						//Kai stulpeliu pradzio bei pabaigos skaitliukai pastumti, kvieciamas metodas, kuris is naujo paslepia bei parodo reikiamus skaitliukus
						this.cols_hide();
			
						
					},
					
					/**
					 * Poslinkio desinen metodas pastumia stulpeliu skaitliukus
					 */
					cols_right_slide: function(){
	
						//Vykdomas stulpeliu postumis ir tikrinama ar stulpeliu blokas neisejo uz rodymo rybu
						if((this.begin_col + this.show_columns)<this.count_columns) this.begin_col = this.begin_col + this.show_columns;
						//Kai stulpeliu pradzio bei pabaigos skaitliukai pastumti, kvieciamas metodas, kuris is naujo paslepia bei parodo reikiamus skaitliukus
						this.cols_hide();
					},
					
					/**
					 * Metodas talpina lenteles suspaudimo ijungimo isjungimo linka pries lentele
					 * Linkas bus priristas prie click evento ir aktyvuos arba deaktyvuos lentu suspaudima
					 */
					hide_on_off_link: function() {
						
						//Jei lenteliu suspaudimo linkai nebuvo sukurti
						if($('.hide_is_on_off[data-table-id="'+this.table_id+'"]').length==0){
							
							//Pagal hide_is_on ijungimo isjungimo flaga kuria atitinkama linka
							if(this.hide_is_on == '0') $('table[data-table-id="'+this.table_id+'"]').before('<a href="javascript:;"  class="hide_is_on_off" data-table-id="'+this.table_id+'">Įjungti lentelės suspaudimą </a><br>');
							if(this.hide_is_on == '1') $('table[data-table-id="'+this.table_id+'"]').before('<a href="javascript:;"  class="hide_is_on_off" data-table-id="'+this.table_id+'">Isjungti lentelės suspaudimą </a><br>');
							
						}
						
						//Jei lenteliu suspaudimo linkai buvo sukurti
						if($('.hide_is_on_off[data-table-id="'+this.table_id+'"]').length>0){
													
							//Pagal hide_is_on ijungimo isjungimo flaga redaguoja linka
							if(this.hide_is_on == '0')  $('.hide_is_on_off[data-table-id="'+this.table_id+'"]').text('Įjungti lentelės suspaudimą');
							if(this.hide_is_on == '1') 	$('.hide_is_on_off[data-table-id="'+this.table_id+'"]').text('Isjungti lentelės suspaudimą');
							
						}

					},

				}
		
			}
		);
		
		//Slepimo inicijavimas visoms rastoms lentelems
		for (var table_id in table_array_objects) {
			
			//Pries kiekviena lentele talpinami suglotninimo ijungimo/isjungimo linkai
			table_array_objects[table_id].hide_on_off_link();
			//Lenteliu slepimas paleidziamas tik tuo atveju, jei rasti atitinkami nurodymai lenteleje
			if(table_array_objects[table_id].hide_is_on == '1') table_array_objects[table_id].cols_hide();
			
			
		}
				
				
		/**
		 * Linko, kuris ijungia lentos suspadima, eventas
		 */
		$('.hide_is_on_off').click(function(event) {
			
			//Gaunamas lenteles_id is linko data tag'o
			table_id = $(event.target).data('table-id');
			
			//Nustatomas flagas, jog slepimas yra ijungtas
			if(table_array_objects[table_id].hide_is_on == '0') {
				
				//Lentos ijungimo flagas nustatomas i priesinga, kad kito paspaudimo metu pasileistu priesingas scenarijus
				table_array_objects[table_id].hide_is_on = 1;
				
				//HTMLe nutatomas flagas, jog slepimas yra isjungtas, 
				$(event.target).data('hide-is-off', '0');
				
				//Paleidziamas lenteles slepimo ijungimo isjungimo linko keitimas
				table_array_objects[table_id].hide_on_off_link();
				
				//Paleidziamas reikiamu stulpeliu slepimo metodas
				table_array_objects[table_id].cols_hide();
				
				//Rodomi rodykliu stulpeliai, jei jie buvo sukurti
				if($('table[data-table-id="'+table_id+'"]').find('.left_arrow').length > 0) $('table[data-table-id="'+table_id+'"]').find('.left_arrow').show();
				if($('table[data-table-id="'+table_id+'"]').find('.right_arrow').length > 0) $('table[data-table-id="'+table_id+'"]').find('.right_arrow').show();
			}
			//Nustatomas flagas, jog slepimas yra isjungtas
			else if(table_array_objects[table_id].hide_is_on == '1') {
				
				//Lentos ijungimo flagas nustatomas i priesinga, kad kito paspaudimo metu pasileistu priesingas scenarijus
				table_array_objects[table_id].hide_is_on = 0;
				
				//HTMLe nutatomas flagas, jog slepimas yra ijungtas
				$(event.target).data('hide-is-off', '1');
				
				//Paleidziamas lenteles slepimo ijungimo isjungimo linko keitimas
				table_array_objects[table_id].hide_on_off_link();
				
				//Paleidziamas reikiamu stulpeliu slepimo metodas
				table_array_objects[table_id].cols_show();
				
				//Slepiami rodykliu stulpeliai, jei jie buvo sukurti
				if($('table[data-table-id="'+table_id+'"]').find('.left_arrow').length > 0) $('table[data-table-id="'+table_id+'"]').find('.left_arrow').hide();
				if($('table[data-table-id="'+table_id+'"]').find('.right_arrow').length > 0) $('table[data-table-id="'+table_id+'"]').find('.right_arrow').hide();
				
			}

		});
		
		/**
		 * Rodykliu stulpelio bei rodykliu linku paspaudimo eventas. 
		 * Veikia per delegavima 
		 * Pirmiausia suveikia eventas, jog paspausta table lenta, po to nustatoma koks lentos ID tada koks stulpelis 
		 * buvo paspaustas
		 */
		$('table').click(function(event){
			
			//Nustatoma kokia rodykle ar desine ar kaire buvo paspausta, nuskaitomas rodykles elementas
			var target_arrow = event.target;
			
			//Id data tag'o nuskaitomas lenteles ID
			var $table_id = $(this).data('table-id');
			
			//Jei paspaustas kaires rodykles stulpelis
			if (target_arrow.className == 'left_arrow' || target_arrow.className == 'left_arrow_link') {

				//Inicijuojamas metodas, kuris vykdo lentos postumi
				table_array_objects[$table_id].cols_left_slide();
			
			}
			
			//Jei paspaustas desines rodykles stulpelis
			if (target_arrow.className == 'right_arrow' || target_arrow.className == 'right_arrow_link') {

				//Inicijuojamas metodas, kuris vykdo lentos postumi
				table_array_objects[$table_id].cols_right_slide();
			
			}
			
		});
		
		
	});
