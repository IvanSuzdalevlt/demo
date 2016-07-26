
	/**
	 * Metodas iesko pirmu sutarciu visose srityse, nustato siu sutarciu periodus 
	 * ir tikrina ar siose perioduose, lentoje kom_periodai_sritys yra uzpildytas laukas procentas_man
	 */
	public static function write_klaidos_kom_periodai_visose_srityse() {
		
		
		//Gaunamos sritys objekto pavydalu
		$sritys_list_object = FsMokSritys::model()->findAll();
		//Ištraukiamos sritys masyvo pavydalu
		$sritys_list_array = CHtml::listdata($sritys_list_object, 'id', 'pavadinimas');	
		//Gaunamas tikrinimo ID
		$tikrinimo_id = Tikrinimai::KOM_PERIODAI_SRITYS_NETURI_PROCENTAS_MAN;
		//Kuriamas naujas DB konstruktorius
		$command=Yii::app()->db->createCommand();
		//Trinami seni irasai aplie kalidas sutartyse pagal atitinkama tikrinimo ID konstanta
		$command->delete('kom_sutarciu_klaidos', 'tikrinimo_id=:tikrinimo_id', array(':tikrinimo_id'=>$tikrinimo_id));
		
		
		$output_object_array = null;;
		//Perrenkamos sritys. Jei kokioje nors srityje
		foreach($sritys_list_array as $srities_id => $srities_pavadinimas) {
			//kviečiant modelio mėtodą, gaunamos objektas, kriamia itkėtina, kad bus informacija apie klaidas
			$error_object = KomPeriodaiSritys::model()->checkErrorsMinPeriodbySritys($srities_id);
			//Jei yra klaidos objektas ir jame laukas procentas_man neturi reikšmės - galima į lentelė dėti duomenys, nes tai klaida
			if(isset($error_object->periodas) and isset($error_object->srities_id) and $error_object->procentas_man == null) {
				
				//Tikrinama ar jau yra toksai masyvo indeksas, jei nera, toksai indeksas sukuriamas
				if(!isset($output_object_array)) $output_object_array = array();
				//Prie kiekvieno periodo pridedami irasai apie klaidas
				$output_object_array[] = $error_object;
				
			}	
		}
		//Kuriamas masyvas, kuriame bus saugomos siejamos su klaida pirmosios periodo ir srities sutartys
		$klaidingos_sutartys_array = array();
		
		//Perrenkamos klaidingos srityse bei siose srityse nustatomos pirmosios sutartys, jei buvo gautas klaidos periode bei srityje
		if(isset($output_object_array) and count($output_object_array)>0){
			
			foreach($output_object_array as $srities_data){
				//Nustatoma pirmoji sutartis srityje				
				$pirma_sutartis = KomPeriodaiSritys::model()->getPirmaSutartisSrityje($srities_data->srities_id);
				//Jei yra atitinkamas masyvo raktas, bus sudaromas pirmuju sutarciu, su kuriuomis siejamos klaidos, masyvas
				if(isset( $pirma_sutartis['id_sutarties'])) {
					
					//Surenkamas masyvas, kuriame raktai bus pirmuju sutarciu ID o reikšmėmis bus klaidų aprašas
					$klaidingos_sutartys_array[$pirma_sutartis['id_sutarties']] = 
										
								" Periode ".$pirma_sutartis['kom_periodas']." yra srities ID ".$srities_data->srities_id." pirmoji sutartis ID ".
								$pirma_sutartis['id_sutarties']." tačiau lentoje kom_periodai_sritys šitam periodui bei sričiai yra neužpildytas laukas procentas_man";
					
				}

			}
		}
		//Kuriamas naujas DB konstruktorius
		$command=Yii::app()->db->createCommand();
		//Perrenkamas sutarciu masyvas ir sutarciu ID bei sutarciu klaidos irasomos i DB
		foreach($klaidingos_sutartys_array as $sutarties_id=>$klaidos_aprasas){
					
			//Viksta rasymas i DB
			$command->insert('kom_sutarciu_klaidos', array(
				'kom_sutarties_id'=>$sutarties_id,
				'flag_aktyvi'=>1,
				'klaida'=> $klaidos_aprasas,
				'tikrinimo_id'=> $tikrinimo_id,
			));
					
		}
	
		return true;		
		
	}
