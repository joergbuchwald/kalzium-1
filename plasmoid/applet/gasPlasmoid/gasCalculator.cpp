/***************************************************************************
    copyright            : (C) 2009 by Kashyap R Puranik
    email                : kashthealien@gmail.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "gasCalculator.h"
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <KTextEdit>
#include <QSlider>
#include <QSpinBox>
#include <KComboBox>
#include <QSizeF>
#include <QLabel>
#include <Plasma/ComboBox>
#include <Plasma/LineEdit>
#include <Plasma/Label>
#include <Plasma/Frame>
#include <Plasma/RadioButton>
#include <Plasma/SpinBox>
#include <Plasma/Slider>
#include <QGraphicsGridLayout>
 
#include <plasma/svg.h>
#include <plasma/theme.h>

using namespace Conversion;

gasCalculator::gasCalculator(QObject *parent, const QVariantList &args)
: Plasma::PopupApplet(parent, args)
, m_widget(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setPopupIcon("accessories-calculator");
    setHasConfigurationInterface(true);
    resize(800, 600);
}

gasCalculator::~gasCalculator()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}
 
void gasCalculator::init()
{
} 
 
 
QGraphicsWidget *gasCalculator::graphicsWidget()
{
//FIXME:
// 1.> Currently the spin boxes are integer, please convert them into double
// and uncomment certain lines of code which say 'setDecimals(4)'
// 2.> The radio buttons allow multiple selection which should not happen, they should be 
// grouped somehow.

	if (!m_widget) {	
		m_widget = new QGraphicsWidget(this);
		
		// setup the label
	    Plasma::Frame *pHeader = new Plasma::Frame(this);
	    pHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	    pHeader->setText(i18n("gas Calculator"));
	    
	    //setup the grid layout
		QGraphicsGridLayout *pGridLayout = new QGraphicsGridLayout(m_widget);
		pGridLayout->addItem(pHeader, 0, 0, 1, 5);
		
		// Set up the user interface
		// molar mass
		
		Plasma::Label *molarMassLabel = new Plasma::Label(this);
	    molarMassLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassLabel->setText(i18n("Molar Mass of Gas:"));
	    
	    m_molarMass = new Plasma::SpinBox(this);
		//m_MolarMass->setDecimals(4);
        m_molarMass->setMaximum(1e+09);
        
		Plasma::Label *molarMassUnitLabel = new Plasma::Label(this);
	    molarMassUnitLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molarMassUnitLabel->setText(i18n("u (mass)"));
	    
	    pGridLayout->addItem(molarMassLabel, 1, 1);
	    pGridLayout->addItem(m_molarMass, 1, 2);
	    pGridLayout->addItem(molarMassUnitLabel, 1, 3);

	    // moles
	    
	    m_r2 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *molesLabel = new Plasma::Label(this);
	    molesLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    molesLabel->setText(i18n("Number of moles:"));
	    
	    m_moles = new Plasma::SpinBox(this);
        //m_Moles->setDecimals(4);
        m_moles->setMaximum(1e+09);
        	    
	    pGridLayout->addItem(m_r2, 2, 0);
	    pGridLayout->addItem(molesLabel, 2, 1);
	    pGridLayout->addItem(m_moles, 2, 2);
	    
	    // mass
	    	    
	    Plasma::Label *massLabel = new Plasma::Label(this);
	    massLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    massLabel->setText(i18n("Mass of the gas:"));
	    
	    m_mass = new Plasma::SpinBox(this);
	    //m_Mass->setDecimals(4);
	    m_mass->setMaximum(1e+09);
	    
  	    m_massUnit = new Plasma::ComboBox(this);
	    m_massUnit->setZValue(2);
		m_massUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("grams", 0)
         << tr2i18n("tons", 0)
         << tr2i18n("carats", 0)
         << tr2i18n("pounds", 0)
         << tr2i18n("ounces", 0)
         << tr2i18n("troy ounces", 0)
        );
        m_massUnit->setZValue(6);
        
		pGridLayout->addItem(massLabel, 3, 1);
	    pGridLayout->addItem(m_mass, 3, 2);
	    pGridLayout->addItem(m_massUnit, 3, 3);

		// pressure
		
		m_r4 = new Plasma::RadioButton(this);
				
		Plasma::Label *pressureLabel = new Plasma::Label(this);
	    pressureLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    pressureLabel->setText(i18n("Pressure of the Gas:"));
	    
	    m_pressure = new Plasma::SpinBox(this);
        //m_Pressure->setDecimals(4);
        m_pressure->setMaximum(1e+09);
        
  	    m_pressureUnit = new Plasma::ComboBox(this);
	    m_pressureUnit->setZValue(2);
		m_pressureUnit->nativeWidget()->insertItems(0, QStringList()
		 << tr2i18n("atmospheres", 0)
         << tr2i18n("pascal", 0)
         << tr2i18n("bars", 0)
         << tr2i18n("millibars", 0)
         << tr2i18n("decibars", 0)
         << tr2i18n("torrs", 0)
         << tr2i18n("inches of mercury", 0)
        );
	    m_pressureUnit->setZValue(5);
	    
	    pGridLayout->addItem(m_r4, 4, 0);
	    pGridLayout->addItem(pressureLabel, 4, 1);
	    pGridLayout->addItem(m_pressure, 4, 2);
	    pGridLayout->addItem(m_pressureUnit, 4, 3);
	    
		// temperature
		
		m_r5 = new Plasma::RadioButton(this);
		
		Plasma::Label *temperatureLabel = new Plasma::Label(this);
	    temperatureLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    temperatureLabel->setText(i18n("Temperature:"));
	    
	    m_temperature = new Plasma::SpinBox(this);
	    //m_Temp->setDecimals(4);
        m_temperature->setMaximum(1e+09);
        
	    m_temperatureUnit = new Plasma::ComboBox(this);
	    m_temperatureUnit->setZValue(2);
	    m_temperatureUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("kelvins", 0)
         << tr2i18n("celsius", 0)
         << tr2i18n("fahrenheit", 0)
         << tr2i18n("delisles", 0)
         << tr2i18n("r\303\251aumurs", 0)
        );
        m_temperatureUnit->setZValue(4);
        
   	    pGridLayout->addItem(m_r5, 5, 0);
	    pGridLayout->addItem(temperatureLabel, 5, 1);
	    pGridLayout->addItem(m_temperature, 5, 2);
	    pGridLayout->addItem(m_temperatureUnit, 5, 3);
	    
	    // volume
	    
	    m_r6 = new Plasma::RadioButton(this);
	    
	    Plasma::Label *volumeLabel = new Plasma::Label(this);
	    volumeLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    volumeLabel->setText(i18n("Volume of the gas:"));
	    
	    m_volume = new Plasma::SpinBox(this);
        //m_volume->setDecimals(4);
        m_volume->setMaximum(1e+09);
        	    
	    m_volumeUnit = new Plasma::ComboBox(this);
	    m_volumeUnit->setZValue(2);
	    m_volumeUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liter", 0)
         << tr2i18n("cubic feet", 0)
         << tr2i18n("cubic inch", 0)
         << tr2i18n("fluid ounce", 0)
         << tr2i18n("cups", 0)
         << tr2i18n("gallons", 0)
         << tr2i18n("pints", 0)
        );
        m_volumeUnit->setZValue(3);
        
	    pGridLayout->addItem(m_r6, 6, 0);
	    pGridLayout->addItem(volumeLabel, 6, 1);
	    pGridLayout->addItem(m_volume, 6, 2);
	    pGridLayout->addItem(m_volumeUnit, 6, 3);
	    
	    // vander waal's constant a
	    
	    Plasma::Label *vand_aLabel = new Plasma::Label(this);
	    vand_aLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    vand_aLabel->setText(i18n("Van der Waals constant 'a':"));
	    
	    m_Vand_a = new Plasma::SpinBox(this);
        //m_Vand_A->setDecimals(4);
        m_Vand_a->setMaximum(1e+09);
        
  	    m_aUnit = new Plasma::ComboBox(this);
	    m_aUnit->setZValue(2);
	    m_aUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liter^2 atmosphere/mol^2", 0)
        );
        m_aUnit->setZValue(2);
        
   	    pGridLayout->addItem(vand_aLabel, 7, 1);
	    pGridLayout->addItem(m_Vand_a, 7, 2);
	    pGridLayout->addItem(m_aUnit, 7, 3);
	    
	    // vander waal's constant b
	    
	    Plasma::Label *vand_bLabel = new Plasma::Label(this);
	    vand_bLabel->nativeWidget()->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
	    vand_bLabel->setText(i18n("Van der Waals constant 'b':"));
	    
	    m_Vand_b = new Plasma::SpinBox(this);
        //m_Vand_B->setDecimals(4);
        m_Vand_b->setMaximum(1e+09);
        
  	    m_bUnit = new Plasma::ComboBox(this);
	    m_bUnit->setZValue(2);
	    m_bUnit->nativeWidget()->insertItems(0, QStringList()
         << tr2i18n("liters", 0)
         << tr2i18n("cubic meters", 0)
         << tr2i18n("cubic feet", 0)
         << tr2i18n("cubic inches", 0)
         << tr2i18n("gallons", 0)
         << tr2i18n("cups", 0)
        );
        m_bUnit->setZValue(1);
        
   	    pGridLayout->addItem(vand_bLabel, 8, 1);
	    pGridLayout->addItem(m_Vand_b, 8, 2);
	    pGridLayout->addItem(m_bUnit, 8, 3);
	    
	    // Results
	    m_error = new Plasma::Label(this);
	    pGridLayout->addItem(m_error, 9, 0, 5, 2);
	    // Adding objects to the UI done, now initialise
	    
	    /**************************************************************************/
	    //                       Gas Calculator set up
	    /**************************************************************************/

	    // initialise the initially selected values
	    m_molarMass  -> setValue(2.008);
	    m_temperature-> setValue(273.0);
	    m_volume     -> setValue(22.400);
	    m_pressure   -> setValue(1.0);
	    m_Vand_a          -> setValue(0.0);
	    m_Vand_b          -> setValue(0.0);
	    m_mass       -> setValue(2.016);
	    m_moles      -> setValue(1.0);
	    // Setup of the UI done

	    // Initialise values
	    m_Temp = Value(273.0, "kelvins");
	    m_MolarMass = 2.016;
	    m_Pressure = Value(1.0, "atmosphere");
	    m_Mass = Value(2.016, "grams");
	    m_Moles = 1.0;
	    m_Vand_A = 0.0;
	    m_Vand_B = Value(0.0, "liters");
	    m_Vol = Value(22.4, "liters");
	    // Initialisation of values done
	    // Connect signals with slots
	    //FIXME replace all int with double after using the doubleSpinBox
	    connect(m_temperature, SIGNAL(valueChanged(int)),
	            this, SLOT(tempChanged()));
	    connect(m_temperatureUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(tempChanged()));
	    connect(m_volume, SIGNAL(valueChanged(int)),
	            this, SLOT(volChanged()));
	    connect(m_volumeUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(volChanged()));
	    connect(m_pressure, SIGNAL(valueChanged(int)),
	            this, SLOT(pressureChanged()));
	    connect(m_pressureUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(pressureChanged()));
	    connect(m_mass, SIGNAL(valueChanged(int)),
	            this, SLOT(massChanged()));
	    connect(m_massUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(massChanged()));
	    connect(m_moles, SIGNAL(valueChanged(int)),
	            this, SLOT(molesChanged(int)));
	    connect(m_molarMass, SIGNAL(valueChanged(int)),
	            this, SLOT(molarMassChanged(int)));
	    connect(m_Vand_a, SIGNAL(valueChanged(int)),
	            this, SLOT(Vand_aChanged()));
	    connect(m_aUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(Vand_aChanged()));
	    connect(m_Vand_b,  SIGNAL(valueChanged(int)),
	            this, SLOT(Vand_bChanged()));
	    connect(m_bUnit->nativeWidget(), SIGNAL(activated(int)),
	            this, SLOT(Vand_bChanged()));
	    /**************************************************************************/
	    // gas Calculator setup complete
	    /**************************************************************************/	    
	}    
    return m_widget;
}

/*
    Note:-

    Van der Val's gas equation
    ( P + n^2 a / V^2) ( V - nb ) = nRT

    where P - pressure
          V - Volume
          n - number of moles
          R - Universal gas constant
          T - temperature

          a,b - Van der Val's constants
*/

// Calculates the Pressure
void gasCalculator::calculatePressure(void)
{
    double pressure;
    double volume = ((Converter::self()->convert(m_Vol, "liters")).number());
    double temp = ((Converter::self()->convert(m_Temp, "kelvins")).number());
    double b = ((Converter::self()->convert(m_Vand_B, "liters")).number());

    pressure = m_Moles * R * temp / (volume - m_Moles * b) - m_Moles * m_Moles * m_Vand_A / volume / volume;
    m_Pressure = Value(pressure, "atmospheres");
    m_Pressure = (Converter::self()->convert(m_Pressure, m_pressureUnit->nativeWidget()->currentText()));
    m_pressure->setValue(m_Pressure.number());

    //pressure =
}

// Calculates the molar mass of the gas
void gasCalculator::calculateMolarMass(void)
{
    double mass = ((Converter::self()->convert(m_Mass, "grams")).number());
    double volume = ((Converter::self()->convert(m_Vol, "liters")).number());
    double pressure = ((Converter::self()->convert(m_Pressure, "atmospheres")).number());
    double temp = ((Converter::self()->convert(m_Temp, "kelvins")).number());
    double b = ((Converter::self()->convert(m_Vand_B, "liters")).number());

    m_MolarMass = mass * R * temp / (pressure + m_Moles * m_Moles * m_Vand_A / volume / volume)\
                  / (volume - m_Moles * b);
    m_molarMass->setValue(m_MolarMass);
}

// Calculates the Volume
void gasCalculator::calculateVol(void)
{
    double volume;
    double pressure = ((Converter::self()->convert(m_Pressure, "atmospheres")).number());
    double temp = ((Converter::self()->convert(m_Temp, "kelvins")).number());
    double b = ((Converter::self()->convert(m_Vand_B, "liters")).number());

    volume = m_Moles * R * temp / pressure + (m_Moles * b);
    m_Vol = Value(volume, "liters");
    m_Vol = (Converter::self()->convert(m_Vol, m_volumeUnit->nativeWidget()->currentText()));
    m_volume->setValue(m_Vol.number());
}

// Calculates the Temperature
void gasCalculator::calculateTemp(void)
{
    double temp;
    double volume = ((Converter::self()->convert(m_Vol, "liters")).number());
    double pressure = ((Converter::self()->convert(m_Pressure, "atmospheres")).number());
    double b = ((Converter::self()->convert(m_Vand_B, "liters")).number());

    temp = (pressure + (m_Moles * m_Moles * m_Vand_A / volume / volume))\
           * (volume - m_Moles * b) / m_Moles / R;
    m_Temp = Value(temp, "kelvins");
    m_Temp = (Converter::self()->convert(m_Temp, m_temperatureUnit->nativeWidget()->currentText()));
    m_temperature->setValue(m_Temp.number());
}

// Calculates the number of moles
void gasCalculator::calculateMoles(void)
{
    double volume = ((Converter::self()->convert(m_Vol, "liters")).number());
    double pressure = ((Converter::self()->convert(m_Pressure, "atmospheres")).number());
    double temp = ((Converter::self()->convert(m_Temp, "kelvins")).number());
    double b = ((Converter::self()->convert(m_Vand_B, "liters")).number());

    m_Moles = (pressure + m_Moles * m_Moles * m_Vand_A / volume / volume)\
              * (volume - m_Moles * b) / R / temp;
    m_moles->setValue(m_Moles);
}

// Calculates the mass of substance
void gasCalculator::calculateMass(void)
{
    double mass;
    double volume = ((Converter::self()->convert(m_Vol, "liters")).number());
    double pressure = ((Converter::self()->convert(m_Pressure, "atmospheres")).number());
    double temp = ((Converter::self()->convert(m_Temp, "kelvins")).number());
    double b = ((Converter::self()->convert(m_Vand_B, "liters")).number());

    mass = (pressure + m_Moles * m_Moles * m_Vand_A / volume / volume)\
           * (volume - m_Moles * b) * m_MolarMass / R / temp;
    m_Mass = Value(mass, "grams");
    m_Mass = (Converter::self()->convert(m_Mass, m_massUnit->nativeWidget()->currentText()));
    m_mass->setValue(m_Mass.number());
}


// Functions ( slots ) that occur on changing a value
// occurs when the volume is changed
void gasCalculator::volChanged()
{
    m_Vol = Value(m_volume->value(), m_volumeUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the temperature is changed
void gasCalculator::tempChanged()
{
    m_Temp = Value(m_temperature->value(), m_temperatureUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the pressure is changed
void gasCalculator::pressureChanged()
{
    m_Pressure = Value(m_pressure->value(), m_pressureUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when the mass is changed
void gasCalculator::massChanged()
{
    m_Mass = Value(m_mass->value(), m_massUnit->nativeWidget()->currentText());
    m_Moles = ((Converter::self()->convert(m_Mass, "grams")).number()) / m_MolarMass;
    m_moles->setValue(m_Moles);
    calculate();
}

//FIXME: change int value to double value in the next two functions after using the doubleSpinBox
// occurs when the number of moles is changed
void gasCalculator::molesChanged(int value)
{
    m_Moles = value;
    m_Mass = Value(m_Moles * m_MolarMass, "grams");
    m_Mass = (Converter::self()->convert(m_Mass, m_massUnit->nativeWidget()->currentText()));
    m_mass->setValue(m_Mass.number());
    calculate();
}

// occurs when the molar mass is changed
void gasCalculator::molarMassChanged(int value)
{
    m_MolarMass = value;
    m_Mass = Value(m_MolarMass * m_Moles, "grams");
    m_Mass = (Converter::self()->convert(m_Mass, m_massUnit->nativeWidget()->currentText()));
    m_mass->setValue(m_Mass.number());
    calculate();
}

// occurs when the number of moles is changed
void gasCalculator::Vand_aChanged()
{
    m_Vand_A = m_Vand_a->value();
    calculate();
}

// occurs when the number of moles is changed
void gasCalculator::Vand_bChanged()
{
    m_Vand_B = Value(m_Vand_b->value(), m_bUnit->nativeWidget()->currentText());
    calculate();
}

// occurs when any quantity is changed
void gasCalculator::calculate(void)
{
    if (m_r2->isChecked()) {
        calculateMoles();
    } else if (m_r4->isChecked()) {
        calculatePressure();
    } else if (m_r5->isChecked()) {
        calculateTemp();
    } else if (m_r6->isChecked()) {
        calculateVol();
    }
}

void gasCalculator::error(int mode)
{
    switch (mode) {
    case VOL_ZERO :
        m_error->setText("Volume cannot be zero, please correct the error!");
    default:
        break;
    }
}

void gasCalculator::debug(void)
{

}
#include "gasCalculator.moc"