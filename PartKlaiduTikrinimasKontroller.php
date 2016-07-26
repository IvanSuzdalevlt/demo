

	/**
	 * Metodas skirtas issaukti klaidu tikrinimo metodus bei irasyti rezultatus i DB
	 */
	public function actionCheckError() {
		
		if(isset($_GET['check']) and ($_GET['check'] == '1' or $_GET['check'] == 'all')){
			
			set_time_limit ( 180 );
			
			$klaidos_buvimas = FsDarbuotojaimokytojai::model()->get_mokytoju_errors_flag();

			$id_patikrinimo = 1;
	
			if(isset($klaidos_buvimas) and $klaidos_buvimas == 1) {
			
				//Ruosiamas periodu masyvas
				$periods_array = FsDarbuotojaimokytojai::model()->get_mokytoju_errors_periods();
				$periods_array = Periods::periods_array_to_intervals($periods_array);
				//Isvedamas periodu masyvas
				$period_string =  Periods::out_period_intervals($periods_array);	
				$klaida = "Už periodus ".$period_string;
				//duomenys saugomi i db
				FsKlaidos::model()->addErrorToDB($id_patikrinimo, $klaida, 1);				
			}
			else FsKlaidos::model()->addErrorToDB($id_patikrinimo, "", 0);
		}
		/***************************************************************************/
		//2
		if(isset($_GET['check']) and ($_GET['check'] == '2' or $_GET['check'] == 'all')){
			
			
			//Cia klaidu tikrinti nereikia, todel nevykdoma jokia salyga			
		}
		/***************************************************************************/
		//3
		if(isset($_GET['check']) and ($_GET['check'] == '3' or $_GET['check'] == 'all')){
			
			set_time_limit ( 180 );
			
			$klaidos_buvimas = KomSchemos::model()->count_klaidos_vientisumas_didejimas();
			
			$id_patikrinimo = 3;
			
			if(isset($klaidos_buvimas) == false or $klaidos_buvimas>0) {
				//Ruosiamas periodu masyvas
				$periods_array = KomSchemos::model()->periodai_klaidos_vientisumas_didejimas();
				$periods_array = Periods::periods_array_to_intervals($periods_array);
				//Isvedamas periodu masyvas
				$period_string = Periods::out_period_intervals($periods_array);
				$klaida = "Už periodus ".$period_string;
				//Duomenys saugomi i db
				FsKlaidos::model()->addErrorToDB($id_patikrinimo, $klaida, 1);
				
			}
			else FsKlaidos::model()->addErrorToDB($id_patikrinimo, "", 0);
			
		}
		/***************************************************************************/
		//4
		if(isset($_GET['check']) and ($_GET['check'] == '4' or $_GET['check'] == 'all')){
			
			
			//Cia klaidu tikrinti nereikia, todel nevykdoma jokia salyga			
		}
		
		/***************************************************************************/
		//5
		if(isset($_GET['check']) and ($_GET['check'] == '5' or $_GET['check'] == 'all')){
			
			set_time_limit ( 180 );
			
			$klaidos_buvimas = KomPeriodaiSritys::model()->get_count_klaidos_kom_periodai_sritys();
			
			$id_patikrinimo = 5;
			
			if(isset($klaidos_buvimas) and $klaidos_buvimas>0) {
				
				//Ruosiamas periodu masyvas
				$periods_array = KomPeriodaiSritys::model()->get_periodai_from_klaidos_kom_periodai_srityse();
				$periods_array = Periods::periods_array_to_intervals($periods_array);
				//Isvedamas periodu masyvas
				$period_string = Periods::out_period_intervals($periods_array);
				
				$klaida = "Už periodus ".$period_string;
				
				//Duomenys saugomi i db
				FsKlaidos::model()->addErrorToDB($id_patikrinimo, $klaida, 1);
				
			}
			else FsKlaidos::model()->addErrorToDB($id_patikrinimo, "", 0);
			
		}
		
