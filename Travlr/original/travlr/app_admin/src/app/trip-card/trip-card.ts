import { Component, OnInit, Input } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Trip } from '../models/trip';
import { Router } from '@angular/router';
import { Authentication } from '../services/authentication';

@Component({
  selector: 'app-trip-card',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './trip-card.html',
  styleUrl: './trip-card.css'
})
export class TripCardComponent implements OnInit {
  @Input() trip!: Trip;
  constructor(private router: Router, private authenticationService: Authentication) { }
  ngOnInit(): void {
  }

  public isLoggedIn() {
    return this.authenticationService.isLoggedIn();
  }

  public editTrip(trip: Trip) {
    localStorage.removeItem('tripCode');
    localStorage.setItem('tripCode', trip.code);
    this.router.navigate(['edit-trip']);
  }

}